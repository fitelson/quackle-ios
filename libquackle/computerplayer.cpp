/*
 *  Quackle -- Crossword game artificial intelligence and analysis tool
 *  Copyright (C) 2005-2019 Jason Katz-Brown, John O'Laughlin, and John Fultz.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>

#include "computerplayer.h"
#include "endgameplayer.h"

using namespace Quackle;

ComputerPlayer::ComputerPlayer()
	: m_name(MARK_UV("Computer Player")), m_id(0), m_dispatch(0)
{
	m_parameters.secondsPerTurn = 10;
    m_parameters.inferring = false;
}

ComputerPlayer::~ComputerPlayer()
{
}

void ComputerPlayer::setDispatch(ComputerDispatch *dispatch)
{
	m_dispatch = dispatch;
	m_simulator.setDispatch(dispatch);
}

void ComputerPlayer::setPosition(const GamePosition &position)
{
	m_simulator.setPosition(position);
}

bool ComputerPlayer::shouldAbort()
{
	return m_dispatch && m_dispatch->shouldAbort();
}

void ComputerPlayer::signalFractionDone(double fractionDone)
{
	if (m_dispatch)
		m_dispatch->signalFractionDone(fractionDone);
}

void ComputerPlayer::considerMove(const Move &move)
{
	m_simulator.addConsideredMove(move);
}

void ComputerPlayer::setConsideredMoves(const MoveList &moves)
{
	m_simulator.setConsideredMoves(moves);
}

MoveList ComputerPlayer::moves(int /* nmoves */)
{
	MoveList ret;
	ret.push_back(move());
	return ret;
}

///////

StaticPlayer::StaticPlayer()
{
	m_name = MARK_UV("Static Player");
	m_id = 1;
}

StaticPlayer::~StaticPlayer()
{
}

Move StaticPlayer::move()
{
	return m_simulator.currentPosition().staticBestMove();
}

MoveList StaticPlayer::moves(int nmoves)
{
	m_simulator.currentPosition().kibitz(nmoves);
	return m_simulator.currentPosition().moves();
}

///////

NormalPlayer::NormalPlayer(double meanLoss, double stdDev, const UVString &name)
	: m_meanLoss(meanLoss), m_stdDev(stdDev)
{
	m_name = name;
	m_id = 200;
}

NormalPlayer::~NormalPlayer()
{
}

Move NormalPlayer::move()
{
	m_simulator.currentPosition().kibitz(50);
	MoveList allMoves = m_simulator.currentPosition().moves();

	if (allMoves.empty())
		return Move::createNonmove();

	double bestEquity = allMoves.front().equity;

	// Clamp target so it never drops below the median candidate
	double medianEquity = allMoves[allMoves.size() / 2].equity;
	double targetEquity = std::max(bestEquity - m_meanLoss, medianEquity);

	// Weight each move by normal PDF centered at targetEquity
	std::vector<double> weights;
	weights.reserve(allMoves.size());
	double sumWeights = 0.0;
	for (const auto &m : allMoves)
	{
		double diff = m.equity - targetEquity;
		double w = std::exp(-0.5 * (diff * diff) / (m_stdDev * m_stdDev));
		weights.push_back(w);
		sumWeights += w;
	}

	if (sumWeights <= 0.0)
		return allMoves.front();

	// Sample from the distribution
	static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
	std::uniform_real_distribution<double> dist(0.0, sumWeights);
	double r = dist(rng);

	double cumulative = 0.0;
	for (size_t i = 0; i < allMoves.size(); ++i)
	{
		cumulative += weights[i];
		if (r <= cumulative)
			return allMoves[i];
	}

	return allMoves.front();
}

MoveList NormalPlayer::moves(int nmoves)
{
	m_simulator.currentPosition().kibitz(nmoves);
	return m_simulator.currentPosition().moves();
}

///////

ScalingDispatch::ScalingDispatch(ComputerDispatch *shadow, double scale, double addition)
	: m_shadow(shadow), m_scale(scale), m_addition(addition)
{
}

bool ScalingDispatch::shouldAbort()
{
	return m_shadow->shouldAbort();
}

void ScalingDispatch::signalFractionDone(double fractionDone)
{
	m_shadow->signalFractionDone(fractionDone * m_scale + m_addition);
}
