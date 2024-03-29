#pragma once

#include <queue>

namespace fatpound::win
{
    class Window;

    namespace io
    {
        class Mouse final
        {
            friend class Window;

        public:
            Mouse() = default;
            Mouse(const Mouse& src) = delete;
            Mouse(Mouse&& src) = delete;
            Mouse& operator = (const Mouse& src) = delete;
            Mouse& operator = (Mouse&& src) = delete;
            ~Mouse() = default;


        public:
            class Event final
            {
            public:
                enum class Type
                {
                    LPress,
                    LRelease,
                    RPress,
                    RRelease,
                    WheelPress,
                    WheelRelease,
                    WheelUp,
                    WheelDown,
                    Enter,
                    Move,
                    Leave,
                    Invalid
                };

            public:
                Event() noexcept;

                Event(Type type, const Mouse& parent) noexcept;

            public:
                std::pair<int, int> GetPos() const noexcept;

                Type GetType() const noexcept;

                int GetPosX() const noexcept;
                int GetPosY() const noexcept;

                bool IsValid() const noexcept;
                bool LeftIsPressed() const noexcept;
                bool RightIsPressed() const noexcept;
                bool WheelIsPressed() const noexcept;

            protected:

            private:
                Type type_;

                int x_;
                int y_;

                bool leftIsPressed_;
                bool rightIsPressed_;
                bool wheelIsPressed_;
            };


        public:
            std::pair<int, int> GetPos() const noexcept;

            Event ReadFromBuffer() noexcept;

            int GetPosX() const noexcept;
            int GetPosY() const noexcept;

            bool IsInWindow() const noexcept;
            bool LeftIsPressed() const noexcept;
            bool RightIsPressed() const noexcept;
            bool WheelIsPressed() const noexcept;
            bool BufferIsEmpty() const noexcept;

            void FlushBuffer() noexcept;


        protected:


        private:
            void OnMouseMove_(int x, int y) noexcept;
            void OnMouseEnter_() noexcept;
            void OnMouseLeave_() noexcept;
            void OnLeftPressed_() noexcept;
            void OnLeftReleased_() noexcept;
            void OnRightPressed_() noexcept;
            void OnRightReleased_() noexcept;
            void OnWheelPressed_() noexcept;
            void OnWheelReleased_() noexcept;
            void OnWheelUp_() noexcept;
            void OnWheelDown_() noexcept;
            void OnWheelDelta_(int delta) noexcept;
            void TrimBuffer_() noexcept;


        private:
            static constexpr unsigned int bufferSize_ = 16u;

            std::queue<Event> buffer_;

            int x_ = 0;
            int y_ = 0;
            int wheelDeltaCarry_ = 0;

            bool isInWindow_ = false;
            bool leftIsPressed_ = false;
            bool rightIsPressed_ = false;
            bool wheelIsPressed_ = false;
        };
    }
}