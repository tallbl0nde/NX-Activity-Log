#include "ui/element/Graph.hpp"
#include "utils/Utils.hpp"

// Padding between xAxis and labels
#define LABEL_PADDING 10
// Y-axis labels font size
#define Y_LABEL_FONT_SIZE 16

namespace CustomElm {
    Graph::Graph(int x, int y, int w, int h, unsigned int n) : Element(x, y, w, h) {
        this->barWidth = 0.5;
        this->labelFont = 16;
        this->showValues = true;

        for (size_t i = 0; i < n; i++) {
            struct Entry e;
            e.bar = new Aether::Rectangle(0, 0, 0, 0);
            e.label = new Aether::Text(0, 0, "", this->labelFont);
            e.value = new Aether::Text(0, 0, "0", this->labelFont);
            this->addElement(e.bar);
            this->addElement(e.label);
            this->addElement(e.value);
            this->column.push_back(e);

            Aether::Rectangle * r = new Aether::Rectangle(this->x(), this->y(), 1, 5);
            this->addElement(r);
            this->sep.push_back(r);
        }

        // Create axis lines
        this->xAxis = new Aether::Rectangle(x, y+h, w, 2);
        this->addElement(this->xAxis);
        this->yAxis = new Aether::Rectangle(x, y, 2, h);
        this->addElement(this->yAxis);

        // Set precision to 0
        this->labelPrecision = 0;
        this->valuePrecision = 0;

        // Init all other vars
        this->barC = Aether::Colour{255, 255, 255, 255};
        this->labelC = Aether::Colour{255, 255, 255, 255};
        this->lineC = Aether::Colour{255, 255, 255, 255};
        this->yMax = 10;
        this->setYSteps(5);
    }

    unsigned int Graph::entries() {
        return this->column.size();
    }

    void Graph::setNumberOfEntries(unsigned int n) {
        if (n < this->column.size()) {
            // Delete excess entries
            while (this->column.size() > n) {
                this->removeElement(this->column[this->column.size() - 1].bar);
                this->removeElement(this->column[this->column.size() - 1].label);
                this->removeElement(this->column[this->column.size() - 1].value);
                this->column.erase(this->column.begin() + this->column.size() - 1);
                this->removeElement(this->sep[this->column.size() - 1]);
                this->sep.erase(this->sep.begin() + this->column.size() - 1);
            }
        } else {
            // If not smaller simply enlarge
            for (size_t i = this->column.size(); i < n; i++) {
                struct Entry e;
                e.bar = new Aether::Rectangle(0, 0, 0, 0);
                e.label = new Aether::Text(0, 0, "", this->labelFont);
                e.value = new Aether::Text(0, 0, "0", this->labelFont);
                e.bar->setColour(this->barC);
                e.label->setColour(this->labelC);
                e.value->setColour(this->barC);
                this->addElement(e.bar);
                this->addElement(e.label);
                this->addElement(e.value);
                this->column.push_back(e);

                Aether::Rectangle * r = new Aether::Rectangle(this->x(), this->y(), 1, 5);
                r->setColour(this->lineC);
                this->addElement(r);
                this->sep.push_back(r);
            }
        }

        this->needsUpdate = true;
    }

    void Graph::setLabel(unsigned int i, std::string s) {
        if (i > this->column.size()) {
            return;
        }

        this->column[i].label->setString(s);

        // Recenter (avoid needing to update everything)
        int mid = this->column[i].label->x() + this->column[i].label->w()/2;
        this->column[i].label->setX(mid - this->column[i].label->w()/2);
    }

    void Graph::setValue(unsigned int i, double v) {
        if (i > this->column.size()) {
            return;
        }

        this->column[i].value->setString(std::to_string(v));
        this->needsUpdate = true;
    }

    void Graph::setBarColour(Aether::Colour c) {
        this->barC = c;
        for (size_t i = 0; i < this->column.size(); i++) {
            this->column[i].bar->setColour(c);
            this->column[i].value->setColour(c);
        }
    }

    void Graph::setLabelColour(Aether::Colour c) {
        this->labelC = c;
        for (size_t i = 0; i < this->column.size(); i++) {
            this->column[i].label->setColour(c);
        }
    }

    void Graph::setLineColour(Aether::Colour c) {
        this->lineC = c;
        for (size_t i = 0; i < this->sep.size(); i++) {
            this->sep[i]->setColour(c);
        }
        for (size_t i = 0; i < this->steps.size(); i++) {
            this->steps[i]->setColour(c);
        }
        this->xAxis->setColour(c);
        this->yAxis->setColour(c);
    }

    void Graph::setBarWidth(float w) {
        if (w >= 0 && w <= 1) {
            this->barWidth = w;
            this->needsUpdate = true;
        }
    }

    void Graph::setFontSize(unsigned int s) {
        this->labelFont = s;
        this->needsUpdate = true;
    }

    void Graph::setLabelPrecision(unsigned int p) {
        if (p != this->labelPrecision) {
            this->labelPrecision = p;
            this->needsUpdate = true;
        }
    }

    void Graph::setValuePrecision(unsigned int p) {
        if (p != this->valuePrecision) {
            this->valuePrecision = p;
            this->needsUpdate = true;
        }
    }

    void Graph::setMaximumValue(double v) {
        this->yMax = v;
        this->needsUpdate = true;
    }

    void Graph::setValueVisibility(bool b) {
        this->showValues = b;
        this->needsUpdate = true;
    }

    void Graph::setYSteps(unsigned int s) {
        if (s < this->steps.size()) {
            // Delete excess entries
            while (this->steps.size() > s) {
                this->removeElement(this->steps[this->steps.size() - 1]);
                this->steps.erase(this->steps.begin() + this->steps.size() - 1);
                this->removeElement(this->yLabel[this->yLabel.size() - 1]);
                this->yLabel.erase(this->yLabel.begin() + this->yLabel.size() - 1);
            }
        } else {
            // If not smaller simply enlarge
            for (size_t i = this->steps.size(); i < s; i++) {
                // Create element for line
                Aether::Rectangle * r = new Aether::Rectangle(this->x(), this->y(), this->w(), 1);
                r->setColour(this->lineC);
                this->addElement(r);
                this->steps.push_back(r);

                // Create label element
                Aether::Text * t = new Aether::Text(this->x(), this->y(), "", Y_LABEL_FONT_SIZE);
                t->setColour(this->labelC);
                this->addElement(t);
                this->yLabel.push_back(t);
            }
        }

        this->needsUpdate = true;
    }

    void Graph::setW(int w) {
        this->needsUpdate = true;
        Element::setW(w);
    }

    void Graph::setH(int h) {
        this->needsUpdate = true;
        Element::setH(h);
    }

    void Graph::update(uint32_t dt) {
        // First update elements pos + size
        if (this->needsUpdate) {
            this->updateElements();
            this->needsUpdate = false;
        }

        // Then pass call down to children
        Element::update(dt);
    }

    void Graph::updateElements() {
        // 0: Update yLabels
        for (size_t i = 0; i < this->yLabel.size(); i++) {
            double val = ((i+1)/(double)this->yLabel.size()) * this->yMax;
            val = Utils::roundToDecimalPlace(val, this->labelPrecision);
            this->yLabel[i]->setColour(this->labelC);
            this->yLabel[i]->setString(Utils::truncateToDecimalPlace(std::to_string(val), this->labelPrecision));
        }

        // 1: Position yAxis line
        int maxW = 0;
        for (size_t i = 0; i < this->yLabel.size(); i++) {
            if (this->yLabel[i]->w() > maxW) {
                maxW = this->yLabel[i]->w();
            }
        }
        this->yAxis->setX(this->x() + maxW + LABEL_PADDING);
        this->yAxis->setRectSize(2, this->h() - this->labelFont - LABEL_PADDING);

        // 2: Position/resize xAxis line
        this->xAxis->setX(this->yAxis->x() + this->yAxis->w());
        this->xAxis->setY(this->yAxis->y() + this->yAxis->h());
        this->xAxis->setRectSize(this->x() + this->w() - this->yAxis->x() - this->yAxis->w(), 2);

        // 3: Position y labels + steps (0: bottom - i: top)
        float gap = (this->xAxis->y() - this->yAxis->y()) / (this->yLabel.size() + 0.5);
        for (size_t i = 0; i < this->yLabel.size(); i++) {
            int y = this->xAxis->y() - (gap*(i+1)) + 1;
            this->yLabel[i]->setX(this->yAxis->x() - this->yLabel[i]->w() - LABEL_PADDING);
            this->yLabel[i]->setY(y - this->yLabel[i]->h()/2);

            this->steps[i]->setX(this->yAxis->x() + this->yAxis->w());
            this->steps[i]->setY(y);
            this->steps[i]->setRectSize(this->x() + this->w() - this->steps[i]->x(), 1);
        }

        // 4: Position each column's elements
        gap = this->xAxis->w() / this->column.size();
        for (size_t i = 0; i < this->column.size(); i++) {
            int x = this->xAxis->x() + (gap*i);
            this->column[i].label->setFontSize(this->labelFont);
            this->column[i].label->setXY(x + 1 + (gap - this->column[i].label->w())/2, this->xAxis->y() + LABEL_PADDING);

            // Cap values larger than max value
            bool isLarger = false;
            double val = std::stod(this->column[i].value->string());
            val = Utils::roundToDecimalPlace(val, this->valuePrecision);
            if (val > this->yMax) {
                isLarger = true;
                val = this->yMax;
            }

            // Recreate bars + values to ensure they're on top of horizontal lines
            if (this->removeElement(this->column[i].bar)) {
                this->column[i].bar = new Aether::Rectangle(0, 0, gap*this->barWidth, (this->xAxis->y() - this->steps[this->steps.size() - 1]->y()) * (val / (float)this->yMax));
                this->column[i].bar->setColour(this->barC);
                this->addElement(this->column[i].bar);
            }
            this->column[i].bar->setXY(x + 1 + (gap - this->column[i].bar->w())/2, this->xAxis->y() - this->column[i].bar->h());
            
            if (this->removeElement(this->column[i].value)) {
                // Format string correctly
                std::string tmp = Utils::truncateToDecimalPlace(std::to_string(val), this->valuePrecision);
                if (isLarger) {
                    tmp += "+";
                }

                // Check for trailing zeroes and if so don't show decimal places
                size_t p = tmp.find(".");
                bool allZero = true;
                if (p != std::string::npos) {
                    for (size_t i = p + 1; i < tmp.length(); i++) {
                        if (tmp[i] != '0') {
                            allZero = false;
                            break;
                        }
                    }
                    if (allZero) {
                        tmp = Utils::truncateToDecimalPlace(tmp, 0);
                    }
                }

                this->column[i].value = new Aether::Text(0, 0, tmp, this->labelFont);
                this->column[i].value->setColour(this->barC);
                this->addElement(this->column[i].value);
            }
            this->column[i].value->setXY(x + 1 + (gap - this->column[i].value->w())/2, this->column[i].bar->y() - this->column[i].value->h() - LABEL_PADDING/2);
            if (val == 0 || !(this->showValues)) {
                this->column[i].value->setHidden(true);
            } else {
                this->column[i].value->setHidden(false);
            }
        }

        // 5: Position x axis separators
        for (size_t i = 0; i < this->sep.size(); i++) {
            int x = this->xAxis->x() + (gap*(i+1));
            this->sep[i]->setXY(x, this->xAxis->y());
        }

        this->yAxis->setRectSize(2, this->yAxis->h() + LABEL_PADDING);
    }
};