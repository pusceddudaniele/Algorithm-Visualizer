#include "AlgorithmVisualizer.hpp"
#include <iostream>

namespace Alg{

    //Public Methods
    bool Visualizer::isRunning(){
        return running;
    }


    Visualizer::Visualizer(Alg::Type alg, int numElements)
        : algType{alg}, numVecElements{numElements} {
        initWindow();
        reset();
    }



    void Visualizer::update(){

            window->clear();

            selectedIndexes = algorithm->selectedElements();
            visualizeVec();
            algorithm->step();

            window->display();
    }



    void Visualizer::handleEvents(){
        sf::Event event;

        //While there are events to process
        while(window->pollEvent(event)){
            switch(event.type){
                //User clicked the button to close the window
                case sf::Event::Closed:
                running = false;
                break;

                //User resized the window
                case sf::Event::Resized:{
                sf::FloatRect newViewRect{0, 0, (float)event.size.width, (float)event.size.height};
                window->setView(sf::View{newViewRect});
                }
                break;

                //User pressed a key
                case sf::Event::KeyPressed:
                switch(event.key.code){
                    //ESC / Q (quit)
                    case sf::Keyboard::Escape:
                    case sf::Keyboard::Q:
                    running = false;
                    break;

                    //R (restart visualization)
                    case sf::Keyboard::R:
                    reset();
                    break;

                    default:
                    break;
                }   //end key pressed switch
                break;

                //Default
                default:
                break;

            }   //end event type switch
        }   //end while
    }



    //PRIVATE METHODS

    void Visualizer::initWindow(){
        window.reset(new sf::RenderWindow{sf::VideoMode(1080, 720), "Algorithm Visualizer"});
        window->setVerticalSyncEnabled(true);
    }



    void Visualizer::reset(){
        //randSeed = time
        randSeed = std::chrono::system_clock::now().time_since_epoch().count();
        initVec();
        initAlg();
    }



    void Visualizer::initVec(){
        vec.resize(numVecElements);

        for(int i = 0; i < numVecElements; i++)
            vec[i] = i + 1;

        std::shuffle(vec.begin(), vec.end(), std::default_random_engine{randSeed});
    }



    void Visualizer::initAlg(){
        switch(algType){
            default:
            case Alg::Type::BubbleSort:
            this->algorithm.reset(new BubbleSort{vec});
            break;

            case Alg::Type::InsertSort:
            this->algorithm.reset(new InsertSort{vec});
            break;

            case Alg::Type::SelectionSort:
            this->algorithm.reset(new SelectionSort{vec});
        }
    }



    void Visualizer::visualizeVec(){
        //Get size of window
        sf::Vector2u windowSize = window->getSize();

        //Size of vector and max element
        const int vecSize = vec.size();
        const auto max = std::max_element(vec.begin(), vec.end());

        //Init rectangle that will represent vec's elements
        sf::RectangleShape rectangle;
        float rectWidth = (float)windowSize.x / vecSize;

        //For each element
        for(int i = 0; i < vecSize; i++){
            //See if it is currently selected
            auto found = std::find(selectedIndexes.begin(), selectedIndexes.end(), i);

            //If it's not, rectangle will be white
            if(found == selectedIndexes.end())
                rectangle.setFillColor(sf::Color::White);

            //Else get the respective color
            else rectangle.setFillColor(colors[(int)(found - selectedIndexes.begin())]);

            //Set rectangle height based on the element's value and window height
            float rectHeight = ((float)vec[i] / *max) * windowSize.y;

            //Set rectangle position based on its size and position in the vector
            sf::Vector2f rectPos{rectWidth * i, windowSize.y - rectHeight};

            //Apply changes to the rectangle and draw it
            rectangle.setSize(sf::Vector2f{rectWidth, rectHeight});
            rectangle.setPosition(rectPos);
            window->draw(rectangle);
        }
    }

}