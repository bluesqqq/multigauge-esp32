#include "graphics/gauge/Element.h"

struct TimeValue {
    float Value;
    unsigned long Time;
};

class Graph : public Element {
    private:
        float secondsDisplayed;
        int bufferMilliseconds;

        Color* backgroundFillColor;
        Color* backgroundStrokeColor;

        Color* secondsStrokeColor;
        Color* graphStrokeColor;

        Value& value;
        std::vector<TimeValue> valueMemory;

    public:
        explicit Graph(YGConfigRef config, Value& val) : Element(config), value(val) {
            secondsDisplayed = 2;
            bufferMilliseconds = 0;
            valueMemory.insert(valueMemory.begin(), {4000, 0});
        }

        void draw(Graphics& g) const override {
            const auto b = getBounds();
            // Get the current time
            unsigned long currentTime = millis();

            size_t valueMemorySize = valueMemory.size();

            // Get the total number of seconds to display;
            float secondLength = b.width / secondsDisplayed;

            //canvas.drawLine(x, mapf(valueMemory.at(0).Value, value.getMinimum(), value.getMaximum(), y + height - 1, y), x + ((currentTime - valueMemory.at(0).Time - bufferMilliseconds) / 1000.0f) * secondLength, mapf(valueMemory.at(0).Value, value.getMinimum(), value.getMaximum(), y + height - 1, y), TFT_WHITE);

            // X position of the right line
            int lineX = b.getRight();

            
            for (int i = 0; i < valueMemorySize; i++)  {
                float currentValue = valueMemory.at(i).Value;
                
                int currentY = mapf(currentValue, value.getMinimum(DEFAULT), value.getMaximum(DEFAULT), b.getBottom(), b.getTop());
                int currentX = b.getLeft() + ((currentTime - valueMemory.at(i).Time) / 1000.0f) * secondLength - (bufferMilliseconds / 1000.0f) * secondLength;

                // Skip the last point as it doesn't have a previous point
                if (i != valueMemorySize - 1) {

                    float previousValue = valueMemory.at(i + 1).Value;

                    int previousY = mapf(previousValue, value.getMinimum(DEFAULT), value.getMaximum(DEFAULT), b.getBottom(), b.getTop());
                    int previousX = b.getLeft() + ((currentTime - valueMemory.at(i + 1).Time) / 1000.0f) * secondLength - (bufferMilliseconds / 1000.0f) * secondLength;

                    // Clipping checks
                    bool leftClip = (currentX < b.getLeft() && previousX > b.getLeft());
                    bool rightClip = (currentX < lineX && previousX > lineX);

                    g.setStroke(0xFFFF);
                    if (currentX >= b.getLeft() && previousX <= lineX) { // In bounds, not clipping
                        // Draw the original line
                        g.strokeLine(currentX, currentY, previousX, previousY);
                    } else if (leftClip && rightClip) { // Clipping both ends 
                        // Calculate the slope of the line
                        float slope = (float)(currentY - previousY) / (currentX - previousX);
                        
                        // Calculate the intersection points
                        int intersectYLeft = previousY + slope * (b.getLeft() - previousX);
                        int intersectYRight = previousY + slope * (lineX - previousX);
                        
                        // Draw the line from the left intersection to the right intersection
                        g.strokeLine(b.getLeft(), intersectYLeft, lineX, intersectYRight);
                    } else if (leftClip) {
                        // Calculate the intersection point
                        float slope = (float)(currentY - previousY) / (currentX - previousX);
                        int intersectY = previousY + slope * (b.getLeft() - previousX);

                        // Draw the line from the line intersection to the previous point
                        g.strokeLine(b.getLeft(), intersectY, previousX, previousY);
                    } else if (rightClip) {
                        // Calculate the intersection point
                        float slope = (float)(currentY - previousY) / (currentX - previousX);
                        int intersectY = previousY + slope * (lineX - previousX);

                        // Draw the line from the current point to the line intersection
                        g.strokeLine(currentX, currentY, lineX, intersectY);
                    }
                }
                // If there is a gap from the currentPoint to the left line draw a straight line to fill the gap
                if (i == 0 && currentX > b.getLeft()){
                    if (currentX > lineX)
                        g.strokeLine(b.getLeft(), currentY, lineX, currentY);
                    else
                        g.strokeLine(currentX, currentY, b.getLeft(), currentY);
                }
            }

            // Draw the seconds tick marks
            for (int i = 0; i < secondsDisplayed; i++) {
                int offset = ((currentTime % 1000) / 1000.0f) * secondLength;
                g.strokeLine(b.getLeft() + offset + (i * secondLength), b.getBottom(), b.getLeft() + offset + (i * secondLength), b.getBottom() + 3);
            }

            g.strokeRect(b.toInt());
            //canvas.drawString(value.getValueString(DEFAULT, true), x, y - 2);
            //canvas.setTextDatum(BL_DATUM);
        }

        void update(int deltaTime) override {
            unsigned long currentTime = millis();
            valueMemory.insert(valueMemory.begin(), {value.getValue(DEFAULT), currentTime});

            // remove expired values
            auto it = std::find_if(valueMemory.begin(), valueMemory.end(), [currentTime, this](const TimeValue& tv) {
                return (currentTime - tv.Time) > (secondsDisplayed * 1000 + bufferMilliseconds);
            });

            if (it != valueMemory.end() && (it + 1) != valueMemory.end()) {
                valueMemory.erase(it + 1, valueMemory.end());
            }
        }
};