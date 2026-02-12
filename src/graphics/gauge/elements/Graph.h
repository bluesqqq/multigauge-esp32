#include "graphics/gauge/Element.h"

#include "graphics/DisplayValue.h"

struct TimeValue {
    float Value;
    unsigned long Time;
};

class Graph : public Element {
    private:
        float secondsDisplayed = 2;
        int bufferMilliseconds = 0;

        FillStroke backgroundColor;

        std::unique_ptr<Color> secondsStrokeColor;
        std::unique_ptr<Color> graphStrokeColor;

        DisplayValue value;
        float minimum = 0.0f;
        float maximum = 100.0f;
        std::vector<TimeValue> valueMemory;

    public:
        Graph(Element* parent, const rapidjson::Value::ConstObject json) : Element(parent, json) {
            if (!json.HasMember("props") || !json["props"].IsObject()) return;
            const rapidjson::Value::ConstObject props = json["props"].GetObject();

            setObj(props, "value", value);

            minimum = value.getMinimumBase();
            maximum = value.getMaximumBase();

            setObj(props, "backgroundColor", backgroundColor);

            if (props.HasMember("secondsColor"))
                secondsStrokeColor = Color::fromJson(props["secondsColor"]);
            
            if (props.HasMember("graphColor"))
                graphStrokeColor = Color::fromJson(props["graphColor"]);

            valueMemory.insert(valueMemory.begin(), {4000, 0});
        }

        void draw(Graphics& g) const override {
            const auto b = getBounds();

            if (backgroundColor.fill) {
                g.setFill(*backgroundColor.fill);
                g.fillRect(b.toInt());
            }

            if (backgroundColor.stroke) {
                g.setStroke(*backgroundColor.stroke);
                g.strokeRect(b.toInt()); 
            }

            // Get the current time
            unsigned long currentTime = millis();

            size_t valueMemorySize = valueMemory.size();

            // Get the total number of seconds to display;
            float secondLength = b.width / secondsDisplayed;

            //canvas.drawLine(x, mapf(valueMemory.at(0).Value, value.getMinimum(), value.getMaximum(), y + height - 1, y), x + ((currentTime - valueMemory.at(0).Time - bufferMilliseconds) / 1000.0f) * secondLength, mapf(valueMemory.at(0).Value, value.getMinimum(), value.getMaximum(), y + height - 1, y), TFT_WHITE);

            // X position of the right line
            int lineX = b.getRight();

            
            if (graphStrokeColor) {
                g.setStroke(*graphStrokeColor);
                for (int i = 0; i < valueMemorySize; i++)  {
                    float currentValue = valueMemory.at(i).Value;
                    
                    int currentY = mapf(currentValue, minimum, maximum, b.getBottom(), b.getTop());
                    int currentX = b.getLeft() + ((currentTime - valueMemory.at(i).Time) / 1000.0f) * secondLength - (bufferMilliseconds / 1000.0f) * secondLength;

                    // Skip the last point as it doesn't have a previous point
                    if (i != valueMemorySize - 1) {

                        float previousValue = valueMemory.at(i + 1).Value;

                        int previousY = mapf(previousValue, minimum, maximum, b.getBottom(), b.getTop());
                        int previousX = b.getLeft() + ((currentTime - valueMemory.at(i + 1).Time) / 1000.0f) * secondLength - (bufferMilliseconds / 1000.0f) * secondLength;

                        // Clipping checks
                        bool leftClip = (currentX < b.getLeft() && previousX > b.getLeft());
                        bool rightClip = (currentX < lineX && previousX > lineX);

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
            }

            if (secondsStrokeColor) {
                g.setStroke(*secondsStrokeColor);
                for (int i = 0; i < secondsDisplayed; i++) {
                    int offset = ((currentTime % 1000) / 1000.0f) * secondLength;
                    g.strokeLine(b.getLeft() + offset + (i * secondLength), b.getBottom(), b.getLeft() + offset + (i * secondLength), b.getBottom() + 3);
                }
            }

            g.setTextColor(rgb(255, 255, 255));
            g.drawText(value.getValueString(), b.getTopLeft().toInt().translated(2, 2), Anchor::TopLeft);
        }

        void update(int deltaTime) override {
            unsigned long currentTime = millis();

            valueMemory.insert(valueMemory.begin(), {value.getValueBase(), currentTime});

            // remove expired values
            auto it = std::find_if(valueMemory.begin(), valueMemory.end(), [currentTime, this](const TimeValue& tv) {
                return (currentTime - tv.Time) > (secondsDisplayed * 1000 + bufferMilliseconds);
            });

            if (it != valueMemory.end() && (it + 1) != valueMemory.end()) {
                valueMemory.erase(it + 1, valueMemory.end());
            }
        }
};

REGISTER_ELEMENT_TYPE("graph", Graph);