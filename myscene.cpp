#include <QString>
#include "scenemanager.h"
#include "screenrenderer.h"
#include "simpleplane.h"
#include "shader.h"
#include "shadertimed.h"
#include "transformation.h"
#include "mousekeyboardcameracontroller.h"
#include "trianglemesh.h"
#include "texture.h"
#include "keyboardtransformation.h"
#include "color.h"
#include "audioengine.h"
#include "soundsource.h"
#include "shadermanager.h"
#include "efx-presets.h"

Node *initScene1();
Node *initScene2();

ScreenRenderer* sortedRenderer;
ScreenRenderer* preOrderRenderer;
unsigned int myScene, myScene2;
SoundSource *gSoundSource1, *gSoundSource2, *gSoundSource3, *gAmbientSoundSource;
Node* gAudioListenerNode;

class SceneSwitcher: public Listener
{
    void keyboard(int key, int modifier)
    {
        if (key == '1')
        {
            sortedRenderer->setScene(myScene);
            gSoundSource1->play();
        }
        else if(key =='2')
        {
            sortedRenderer->setScene(myScene2);
            gSoundSource2->play();
        }
        else if (key == 'l' || key == 'L')
        {
            gSoundSource3->play();
        }
    }
};

void SceneManager::initScenes()
{
    SceneSwitcher* lSwitch = new SceneSwitcher();
    Camera* cam = new Camera();
    CameraController* camController = new MouseKeyboardCameraController(cam);
    Q_UNUSED(camController);
    RenderingContext* myContext = new RenderingContext(cam);
    unsigned int myContextNr = SceneManager::instance()->addContext(myContext);
    myScene = SceneManager::instance()->addScene(initScene1());
    sortedRenderer = new ScreenRenderer(myContextNr, myScene);
    Q_UNUSED(lSwitch);

    //Vorsicht: Die Szene muss initialisiert sein, bevor das Fenster verändert wird (Fullscreen)
    SceneManager::instance()->setActiveScene(myScene);
    SceneManager::instance()->setActiveContext(myContextNr);
    SceneManager::instance()->setFullScreen();
}


Node *initScene1()
{
    QString path(SRCDIR); //aus .pro-File!
    //Objekte anlegen
    Drawable *p = new Drawable(new SimplePlane(3.0));
    Drawable *p1 = new Drawable(new SimplePlane(3.0, 3.0, 100.0));
    Node *root=new Node();
    Texture *t;

    QString shaderPath(path+"/");

    Shader* s = ShaderManager::getShader(path + QString("/shaders/Filter.vert"), path + QString("/shaders/Filter.frag"));
    p->setShader(s);
    ShaderTimed* ltimedShader =
      ShaderManager::getShader<ShaderTimed>(path + QString("/shaders/Filter.vert"), path + QString("/shaders/Filter.frag"));
    ltimedShader->setMsecsPerIteration(1000);
    p1->setShader(ltimedShader);
    // Nodes anlegen
    Node* pNode = new Node(p);
    Node* p1Node = new Node(p1);


    //Texturen laden
    t = p->getProperty<Texture>();
    t->loadPicture(path + QString("/modelstextures/l.bmp"));
    //t->makeStripes(256, 10);
    t = p1->getProperty<Texture>();
    t->makeStripes(256, 10);



    root->addChild(pNode);
    root->addChild(p1Node);




    return(root);
}
