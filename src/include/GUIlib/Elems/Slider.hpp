#pragma once

#include <GUIlib/HIDhandler.hpp>
#include <AdtClasses/AdtClasses.hpp>
#include <functional>

namespace GUIlib{

    class Slider{
        public:
        
        vec2 posPoint1, posPoint2, circlePos, value, sliderPos;
        vec2 pos = vec2(0, 0);

        string name;

        vec3 colorLine, circleColor, rectColor;
        float circleRadius, rectLen, height, transperancy = 255;
        CircleShape circle;
        RectangleShape rect, sliderRect;

        function<void(Slider*, vec2&)> func;
        GUIlib::Button btn;

        int type;

        Slider(string name, vec2 posPoint1, vec2 posPoint2, vec3 colorLine, vec3 circleColor, float circleRadius){
            this->name=name;
            this->posPoint1=posPoint1;
            this->posPoint2=posPoint2;
            this->colorLine=colorLine;
            this->circleColor=circleColor;
            this->circleRadius=circleRadius;
            this->func=func;
            type = 0;

            circle.setRadius(circleRadius);
            circle.setFillColor({circleColor.x, circleColor.y, circleColor.z});

            circlePos = {(posPoint1.x == posPoint2.x) ? posPoint1.x - circleRadius : 0, (posPoint1.x == posPoint2.x) ? 0  - circleRadius: posPoint1.y};
            circlePos = {(posPoint1.x == posPoint2.x) ? circlePos.x : clamp(posPoint1.x, posPoint2.x-circleRadius*2, circlePos.x), (posPoint1.x == posPoint2.x) ? clamp(posPoint1.y, posPoint2.y-circleRadius, circlePos.y) : circlePos.y - circleRadius};
            btn.size = {circleRadius*2, circleRadius*2};
            btn.visible = false;
        }

        Slider(string name, vec2 posPoint1, vec2 posPoint2, vec3 colorLine, vec3 rectColor, float rectLen, float height){
            this->name=name;
            this->posPoint1=posPoint1;
            this->posPoint2=posPoint2;
            this->colorLine=colorLine;
            this->rectColor=rectColor;
            this->rectLen=rectLen;
            this->height=height;

            if(posPoint1.x != posPoint2.x){
                rect.setSize({posPoint2.x - posPoint1.x, height});
                sliderRect.setSize({rectLen, height});
                rect.setFillColor({colorLine.x, colorLine.y, colorLine.z});
                rect.setPosition({posPoint1.x, posPoint1.y});
            } else {
                rect.setSize({height, posPoint2.y - posPoint1.y});
                sliderRect.setSize({height, rectLen});
                rect.setFillColor({colorLine.x, colorLine.y, colorLine.z});
                rect.setPosition({posPoint1.x, posPoint1.y});
            }
                
            if(posPoint1.x != posPoint2.x){
                sliderPos = {-rectLen / 2, posPoint1.y};
                sliderPos = pos+vec2(clamp(posPoint1.x, posPoint2.x - rectLen, sliderPos.x), posPoint1.y);
            } else {
                sliderPos = {posPoint1.x, -rectLen / 2};
                sliderPos = pos+vec2(posPoint1.x, clamp(posPoint1.y, posPoint2.y - rectLen, sliderPos.y));
            }
            
            btn.size = {sliderRect.getSize().x, sliderRect.getSize().y};
            btn.visible = false;
            type = 1;
        }

        void setPos(vec2 &pos){
            this->pos = pos;
        }

        void setFunc(function<void(Slider*, vec2&)> func){
            this->func=func;
        }

        void getValue(){
            if(type == 0) value = (circlePos - posPoint1) / ((posPoint2 - circleRadius*2) - posPoint1);
            else {
                value = (sliderPos - posPoint1) / (posPoint2 - rectLen - posPoint1);
            }
        }

        vec2 &returnValue(){
            return value;
        }

        float getRightValue(){
            return (posPoint2.x == posPoint1.x) ? value.y : value.x;
        }

        void draw(RenderWindow &window, MouseData &mdata){
            if(type == 0){
            
                Vertex line[] =
                {
                    Vertex({pos.x+posPoint1.x, pos.y+posPoint1.y}, {colorLine.x, colorLine.y, colorLine.z, transperancy}),
                    Vertex({pos.x+posPoint2.x, pos.y+posPoint2.y}, {colorLine.x, colorLine.y, colorLine.z, transperancy})
                };
                window.draw(line, 2, Lines);
                btn.pos = pos+circlePos;
                if(btn.isHolded(getMousePos(window), 0)){
                    vec2 tempPos = circlePos;
                    circlePos = vec2((posPoint1.x == posPoint2.x) ? posPoint1.x - circleRadius : getMousePos(window).x, (posPoint1.x == posPoint2.x) ? getMousePos(window).y  - circleRadius: posPoint1.y);
                    circlePos = pos+vec2((posPoint1.x == posPoint2.x) ? circlePos.x : clamp(posPoint1.x, posPoint2.x - circleRadius*2, circlePos.x - circleRadius), (posPoint1.x == posPoint2.x) ? clamp(posPoint1.y, posPoint2.y-circleRadius*2, circlePos.y) : circlePos.y - circleRadius);

                    if(circlePos != tempPos){
                        getValue();
                        if(func)
                            func(this, returnValue());
                    }
                }
                circle.setFillColor({circleColor.x, circleColor.y, circleColor.z, transperancy});
                circle.setPosition({circlePos.x, circlePos.y});
                
                window.draw(circle); 
            } else {
                window.draw(rect);
                btn.pos = pos+sliderPos;
                if(btn.isHolded(getMousePos(window), 0)){
                    vec2 tempPos = sliderPos;
                    if(posPoint1.x != posPoint2.x){
                        sliderPos = {getMousePos(window).x - rectLen / 2, posPoint1.y};
                        sliderPos = pos+vec2(clamp(posPoint1.x, posPoint2.x - rectLen, sliderPos.x), posPoint1.y);
                    } else {
                        sliderPos = {posPoint1.x, getMousePos(window).y - rectLen / 2};
                        sliderPos = pos+vec2(posPoint1.x, clamp(posPoint1.y, posPoint2.y - rectLen, sliderPos.y));
                    }

                    if(sliderPos != tempPos){
                        getValue();
                        if(func)
                            func(this, returnValue());
                    }
                }
                sliderRect.setPosition({sliderPos.x, sliderPos.y});
                sliderRect.setFillColor({rectColor.x, rectColor.y, rectColor.z, transperancy});
                rect.setFillColor({colorLine.x, colorLine.x, colorLine.x, transperancy});
                window.draw(sliderRect);   
            }
        }

        bool operator==(const Slider &other) const {
            return name == other.name;
        }

        Slider(){
            ;
        }
    };

}