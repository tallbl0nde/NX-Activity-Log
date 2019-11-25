#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

#include <cstdint>
#include "theme.hpp"

namespace UI {
    // A drawable is an abstract class that is inherited
    // by any type of drawable object. It stores the object's
    // position, size and theme (theme is shared between all objects)
    class Drawable {
        protected:
            // Theme
            static Theme * theme;

            // Position and size (int to allow negative positioning)
            int x;
            int y;
            int w;
            int h;

        public:
            // Constructor takes position and size
            // If none provided defaults to:
            // x = 0, y = 0, w = 100, h = 100
            Drawable(int = 0, int = 0, int = 100, int = 100);

            // Update method can be declared in children
            virtual void update(uint32_t);

            // Draw method must be declared in children
            virtual void draw() = 0;

            // Default destructor does nothing
            virtual ~Drawable();

            // Getters and setters
            int getX();
            int getY();
            int getW();
            int getH();
            virtual void setX(int);
            virtual void setY(int);
            virtual void setW(int);
            virtual void setH(int);
    };
};

#endif