#ifndef ELEMENT_GRAPH_HPP
#define ELEMENT_GRAPH_HPP

#include "Aether/Aether.hpp"

// Stores info required for each "bar"/entry (within elements)
struct Entry {
    Aether::Rectangle * bar;
    Aether::Text * label;
    Aether::Text * value;
};

namespace CustomElm {
    // A Graph only handles the actual graph and not any titles/headings.
    // Draws a simple bar graph.
    class Graph : public Aether::Element {
        private:
            // Vector of Entry struct
            std::vector<Entry> column;
            // Vector of x-axis separating lines
            std::vector<Aether::Rectangle *> sep;
            // Vector of y-axis steps
            std::vector<Aether::Rectangle *> steps;
            // Vector of y-axis labels
            std::vector<Aether::Text *> yLabel;

            // Main axis lines
            Aether::Rectangle * xAxis;
            Aether::Rectangle * yAxis;

            // Colours (all set white initially)
            Aether::Colour barC;
            Aether::Colour labelC;
            Aether::Colour lineC;

            // Bar width in percent of column
            float barWidth;
            // Label font size
            unsigned int labelFont;
            // Whether to show values
            bool showValues;

            // Precisions
            unsigned int labelPrecision;
            unsigned int valuePrecision;

            // Y-axis properties
            double yMax;

            // Set to true to call below function (avoid a lot of redraws)
            bool needsUpdate;
            // Repositions + resizes ALL elements
            void updateElements();

        public:
            // x, y, w, h + number of elements to initialise (can be changed later)
            Graph(int, int, int, int, unsigned int);

            // Returns number of entries/columns
            unsigned int entries();
            // Change number of entries
            // Increasing will set new entries "empty"
            // Decreasing will delete excess entries!
            void setNumberOfEntries(unsigned int);

            // Set the characteristics of each entry (pass index starting from 0 as first parameter)
            // Does nothing if outside range
            void setLabel(unsigned int, std::string);
            void setValue(unsigned int, double);

            // Set the colour of all bars (+ values)
            void setBarColour(Aether::Colour);
            // Set the colour of all labels
            void setLabelColour(Aether::Colour);
            // Set the colour of all lines
            void setLineColour(Aether::Colour);

            // Set the width of each bar (0 - 1)
            void setBarWidth(float);
            // Set font size of all labels
            void setFontSize(unsigned int);
            // Whether to show values above bars
            void setValueVisibility(bool);

            // Set number of decimal points for label
            void setLabelPrecision(unsigned int);
            // Set number of decimal points for value
            // Doesn't update current values!!
            void setValuePrecision(unsigned int);
            // Set maximum y-axis value
            void setMaximumValue(double);
            // Set number of steps on y-axis
            void setYSteps(unsigned int);

            // Changing dimensions must reposition elements
            void setW(int);
            void setH(int);

            // Elements will be repositioned on update if need be
            void update(uint32_t);
    };
};

#endif