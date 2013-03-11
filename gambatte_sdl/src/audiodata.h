/***************************************************************************
 *   Copyright (C) 2007 by Sindre Aamås                                    *
 *   sinamas@users.sourceforge.net                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2 as     *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License version 2 for more details.                *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   version 2 along with this program; if not, write to the               *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef AUDIODATA_H
#define AUDIODATA_H

#include "ringbuffer.h"
#include "rateest.h"
#include "scoped_ptr.h"
#include <SDL.h>
#include <cstddef>

class AudioData {
public:
	struct Status {
		long fromUnderrun;
		long fromOverflow;
		long rate;

		Status(long fromUnderrun, long fromOverflow, long rate)
		: fromUnderrun(fromUnderrun), fromOverflow(fromOverflow), rate(rate)
		{
		}
	};

	AudioData(unsigned sampleRate, unsigned latency, unsigned periods);
	~AudioData();
	Status write(Sint16 const *inBuf, std::size_t samples);

private:
	struct SdlDeleter;

	RingBuffer<Sint16> rbuf_;
	RateEst rateEst_;
	scoped_ptr<SDL_mutex, SdlDeleter> const mut_;
	scoped_ptr<SDL_cond, SdlDeleter> const bufReadyCond_;
	bool failed_;

	static void fillBuffer(void *data, Uint8 *stream, int len) {
		static_cast<AudioData *>(data)->read(stream, len);
	}

	void read(Uint8 *stream, std::size_t len);
};

#endif
