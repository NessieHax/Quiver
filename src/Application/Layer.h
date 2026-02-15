#pragma once


class Layer
{
private:
    /* data */
public:
    Layer(/* args */);
    ~Layer();
 
    virtual void OnAttach(void);
    virtual void OnUpdate(void);
    virtual void OnDetach(void);
};
