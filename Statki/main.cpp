#include <irrlicht.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace irr;
using namespace std;

class Statek
{
public:
    int GetLocX() { return _locX;}
    int GetLocY() { return _locY;}
    int GetDestX() { return _destX;}
    int GetDestY() { return _destY;}
    int GetReloadStatus() { return _reloadStatus;}
    void ReloadStart()
    {
        _reloadStatus = _reloadTime;
    }
    void ReloadContinue()
    {
        _reloadStatus--;
    }
    void DestChange(int portX[], int portY[])
    {
        int i = rand() % 7;
        _destX = portX[i];
        _destY = portY[i];
    }
    bool IsDestReached()
    {
        if ((_locX == _destX) && (_locY == _destY)) return true;
        else return false;
    }
    virtual void Draw(video::IVideoDriver &driver, video::ITexture &Ttankowiec, video::ITexture &Tdrobnicowiec) {}
    virtual void Draw2(video::IVideoDriver &driver, video::ITexture &Ttankowiec, video::ITexture &Tdrobnicowiec) {}
    virtual void Go() {}
protected:
    int _locX, _locY, _destX, _destY,
    _reloadTime, _reloadStatus, _speed,
    portX[7], portY[7];

    Statek(int portX[], int portY[])
    {
        _locX = rand() % 640;
        _locY = rand() % 640;
        _reloadStatus = 0;
        DestChange(portX, portY);
    }

private:
    Statek() {}
};

class Tankowiec : public Statek
{
public:
    Tankowiec(int portX[], int portY[]) : Statek(portX, portY)
    {
        _speed = 1;
        _reloadTime = 80;
    }
    void Draw(video::IVideoDriver &driver, video::ITexture &Ttankowiec, video::ITexture &Tdrobnicowiec)
    {
        driver.draw2DImage(&Ttankowiec, core::position2d<s32>(_locX - 50, _locY - 33.5), core::rect<s32>(0, 0, 100, 67), 0, video::SColor(255, 255, 255, 255), true);
    }
    void Draw2(video::IVideoDriver &driver, video::ITexture &Ttankowiec, video::ITexture &Tdrobnicowiec)
    {
        driver.draw2DImage(&Ttankowiec, core::position2d<s32>(_locX - 50, _locY - 33.5), core::rect<s32>(0, 0, 100, 67), 0, video::SColor(255, 128, 128, 128), true);
    }
    void Go()
    {
        if (_locX != _destX)
            if (_locX < _destX) _locX += _speed;
            else _locX -= _speed;
        else
            if (_locY < _destY) _locY += _speed;
            else _locY -= _speed;
    }
};

class Drobnicowiec : public Statek
{
public:
    Drobnicowiec(int portX[], int portY[]) : Statek(portX, portY)
    {
        _speed = 1;
        _reloadTime = 50;
    }
    void Draw(video::IVideoDriver &driver, video::ITexture &Ttankowiec, video::ITexture &Tdrobnicowiec)
    {
        driver.draw2DImage(&Tdrobnicowiec, core::position2d<s32>(_locX - 50, _locY - 37.5), core::rect<s32>(0, 0, 100, 75), 0, video::SColor(255, 255, 255, 255), true);
    }
    void Draw2(video::IVideoDriver &driver, video::ITexture &Ttankowiec, video::ITexture &Tdrobnicowiec)
    {
        driver.draw2DImage(&Tdrobnicowiec, core::position2d<s32>(_locX - 50, _locY - 37.5), core::rect<s32>(0, 0, 100, 75), 0, video::SColor(255, 128, 128, 128), true);
    }
    void Go()
    {
        if (_locY != _destY)
            if (_locY < _destY) _locY += _speed;
            else _locY -= _speed;
        else
            if (_locX < _destX) _locX += _speed;
            else _locX -= _speed;
    }
};

class Ports
{
public:
    Ports(int id)
    {
        _portX = rand() % 640;
        _portY = rand() % 640;
        _id = id;
    }
    int GetX() { return _portX;}
    int GetY() { return _portY;}
    void Draw(video::IVideoDriver &driver, gui::IGUIFont &font)
    {
        driver.draw2DRectangle(video::SColor(255, 31, 71, 127), core::rect<s32>(_portX - 15, _portY - 15, _portX + 15, _portY + 15));
        //numerki
    }
private:
    int _portX, _portY, _id;
};

int main()
{
    // ------- PREPARING ----------------------------

    // --- DEVICE ---
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 640));
    if (device == 0) return 1;

    // --- TITLE ---
    device->setWindowCaption(L"Statki");

    // --- DRIVER ---
    video::IVideoDriver *driver = device->getVideoDriver();

    // --- TEXTURE ---
    video::ITexture *water = driver->getTexture("media/water.png");
    driver->makeColorKeyTexture(water, core::position2d<s32>(0, 0));

    core::rect<s32> water1(640, 0, 1280, 640);
    core::rect<s32> water2(0, 640, 640, 1280);

    video::ITexture *Ttankowiec = driver->getTexture("media/tankowiec.png");
    driver->makeColorKeyTexture(Ttankowiec, core::position2d<s32>(0, 0));
    video::ITexture *Tdrobnicowiec = driver->getTexture("media/drobnicowiec.png");
    driver->makeColorKeyTexture(Tdrobnicowiec, core::position2d<s32>(0, 0));

    // --- FONT ---
    gui::IGUIFont *font = device->getGUIEnvironment()->getBuiltInFont();

    // --- FILTERING ---
    driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
    driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;

    // ----------------------------------------------

    // ------- SCENES -------------------------------

    srand(time(NULL));

    // --- PREPARING OBJECTS ---
    Ports *ports[7];
    for (int i = 0; i < 7; i++)
    {
        ports[i] = new Ports(i);
    }

    int portX[7], portY[7];
    for (int i = 0; i < 7; i++)
    {
        portX[i] = ports[i]->GetX();
        portY[i] = ports[i]->GetY();
    }

    Statek *statek[5];
    for (int i = 0; i < 2; i++)
    {
        statek[i] = new Tankowiec(portX, portY);
    }

    for (int i = 2; i < 5; i++)
    {
        statek[i] = new Drobnicowiec(portX, portY);
    }

    // --- SCENE ---
    while (device->run() && driver)
    {
        if (device->isWindowActive())
        {
            // --- TIMER ---
            u32 time = device->getTimer()->getTime();

            if (time % 250 == 0)
            {
                // --- SCENE DRAWING ---
                driver->beginScene(true, true, video::SColor(255, 255, 255, 255));

                driver->draw2DImage(water, core::position2d<s32>(0, 0), (time/250 % 2) ? water1 : water2, 0, video::SColor(255, 255, 255, 255), true);

                // --- PORTS DRAWING
                for (int i = 0; i < 7; i++)
                {
                    ports[i]->Draw(*driver, *font);
                }

                // --- SHIPS DRAWING AND MOVING ---
                for (int i = 0; i < 5; i++)
                {
                    statek[i]->Draw(*driver, *Ttankowiec, *Tdrobnicowiec);
                    if (statek[i]->IsDestReached())
                    {
                        statek[i]->Draw2(*driver, *Ttankowiec, *Tdrobnicowiec);
                        if (statek[i]->GetReloadStatus() > 0)
                        {
                            if (statek[i]->GetReloadStatus() == 1) statek[i]->DestChange(portX, portY);
                            statek[i]->ReloadContinue();
                        }
                        else statek[i]->ReloadStart();
                    }
                    else
                    {
                        statek[i]->Draw(*driver, *Ttankowiec, *Tdrobnicowiec);
                        statek[i]->Go();
                    }
                }

                // --- END SCENE ---
                driver->endScene();
            }
        }
    }

    delete[] statek;
    delete[] ports;
    device->drop();
    return 0;
}
