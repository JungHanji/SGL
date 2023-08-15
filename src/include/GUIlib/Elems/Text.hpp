#pragma once

#include <GUIlib/HIDhandler.hpp>
#include <AdtClasses/AdtClasses.hpp>
#include <functional>

namespace GUIlib{

    class Text{
        public:

        wstring text;
        string name;
        vector<wstring> lines;
        vector<sf::Text> linesSF;
        vec2 *psize, *ppos;
        
        Font sffont;
        sf::Text sftext;
        vec3 textColor, backgroundColor;
        vec2 backGroundSize = {0, 0}, pos, padding = vec2(0, 0);
        RectangleShape backGround;

        Figure figure;

        vec2 globalPadding = vec2(0, 0);

        bool enabledLines = false, bg_enabled, usingFigure = false;
        int charSize = 20, typeTextPosition = 0;
        float transparency = 255;

        Text(string name, wstring text, vec3 textColor, vec3 backColor, vec2 bgSize, vec2 pos, string font, int size = 20){
            this->name = name;
            this->text = text;
            this->textColor = textColor;
            this->backgroundColor = backColor;
            this->backGroundSize = bgSize;
            this->pos = pos;
            this->charSize = size;

            sffont.loadFromFile(font);
            sftext.setCharacterSize(size);
            sftext.setFillColor({textColor.x, textColor.y, textColor.z, transparency});
            sftext.setString(text);
            backGround.setSize({bgSize.x, bgSize.y});
            backGround.setFillColor({backColor.x, backColor.y, backColor.z, transparency});
            backGround.setPosition({pos.x, pos.y});

            bg_enabled = true;
        }

        Text(string name, wstring text, vec3 textColor, vec2 pos, string font, int size = 20){
            this->name = name;
            this->text = text;
            this->textColor = textColor;
            this->pos = pos;
            this->charSize = size;

            sffont.loadFromFile(font);
            sftext.setCharacterSize(size);
            sftext.setFillColor({textColor.x, textColor.y, textColor.z, transparency});
            sftext.setString(text);
            bg_enabled = false;
        }

        void setFigure(Figure f){
            bg_enabled = false;
            usingFigure = true;
            figure = f;
            f.pos = pos;
        }

        void changeText(wstring text){
            this->text = text;
            sftext.setString(text);
        }

        void enableLines(bool enable){
            this->enabledLines = enable;
        }

        void addLine(wstring line){
            lines.push_back(line);
            linesSF.push_back(sf::Text(line.substr(1), sffont, charSize));
        }

        void changeLine(int index, wstring newLine){
            if(index == -1) index = lines.size() - 1;
            lines[index] = newLine;
            linesSF[index].setString(newLine);
        }

        void removeLine(int index){
            if(index == -1) index = lines.size() - 1;
            lines.erase(lines.begin() + index);
            linesSF.erase(linesSF.begin() + index);
        }

        wstring getLine(int index){
            if(index == -1) index = lines.size() - 1;
            return lines[index];
        }

        void draw(sf::RenderWindow& window, int tick){
            if(bg_enabled) window.draw(backGround);
            if(usingFigure) figure.draw(window);
            if(!enabledLines){
                if(!(bool)(tick)) sftext.setFont(sffont);
                
                if(typeTextPosition == 0) sftext.setPosition({pos.x + backGroundSize.x/2 - sftext.getLocalBounds().width/2 + padding.x, pos.y + backGroundSize.y/2 - sftext.getLocalBounds().height/2  + padding.y});
                else if(typeTextPosition == 1){
                    sftext.setPosition({pos.x + backGroundSize.x/2 + padding.x, pos.y + backGroundSize.y/2 - sftext.getLocalBounds().height/2 + padding.y});
                }

                window.draw(sftext);
            } else {
                for(int i = 0; i < lines.size(); i++){
                    if(!(bool)(tick)) linesSF[i].setFont(sffont);
                    
                    if(typeTextPosition == 0) linesSF[i].setPosition({pos.x + backGroundSize.x/2 - linesSF[i].getLocalBounds().width/2 + padding.x, pos.y + (backGroundSize.y/2 * i * 0.2) + padding.y});
                    else if(typeTextPosition == 1){
                        linesSF[i].setPosition({pos.x + padding.x, pos.y + (backGroundSize.y/2 * i * 0.2) + padding.y});
                    }

                    window.draw(linesSF[i]);
                }
            }
        }

        bool operator==(const Text &other) const {
            return name == other.name;
        }

        Text(){
            ;
        }
    };

}