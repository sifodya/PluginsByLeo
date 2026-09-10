//
// Created by vboxuser on 06.08.2026.
//

#pragma once
#include <stdexcept>

namespace LeoExceptions
{
    enum LEO_RETURN
    {
        LEO_SUCCESS = 0,
        LEO_ERROR = 1,
        LEO_FAILURE = 2,
        LEO_INVALID_ARGUMENT = 3,
        LEO_NOT_FOUND = 4,
        LEO_NOT_IMPLEMENTED = 5,
        LEO_OUT_OF_RANGE = 6,
        LEO_OUT_OF_MEMORY = 7,
        LEO_OUT_OF_BOUNDS = 8,
        LEO_PLAYHEAD_STOP = 9,
        LEO_PLAYHEAD_START = 10,
        LEO_PLAYHEAD_PLAYING = 11

    };

    /*enum LEO_PLAYHEAD_STATE
    {
        LEO_PLAYHEAD_STOP = 0,
        LEO_PLAYHEAD_PAUSE = 1,
        LEO_PLAYHEAD_PLAYING = 2,
    };

    class LeoThrow : public std::exception
    {

    };

    class LeoPLayhead : public std::exception
    {
        private:
        std::string m_message;
        public:
        LeoPLayhead(const LEO_PLAYHEAD_STATE state)
        {
            switch (state)
            {
            case LEO_PLAYHEAD_STOP:
                m_message = "Playhead is stopped";
                break;
            case LEO_PLAYHEAD_PAUSE:
                m_message = "Playhead is paused";
                break;
                case LEO_PLAYHEAD_PLAYING:
                m_message = "Playhead is playing";
                break;
            }
        };

        const char * what() const noexcept override
        {
            return m_message.c_str();
        }
    };*/
}
