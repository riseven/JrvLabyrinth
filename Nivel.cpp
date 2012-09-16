#include <allegro.h>
#include <alleggl.h>
#include "Nivel.h"

#include "Excepcion.h"
#include "Camara.h"
#include "Jugador.h"
#include "Bloque.h"
#include "Puerta.h"
#include "Losa.h"
#include "Ascensor.h"
#include "Orientacion.h"
#include "EtapaRender.h"
#include "Luz.h"

int Nivel::glTexGui ;
int Nivel::glTexNivel ;
int Nivel::glTexNum[10] ;
int Nivel::glTexTiempo ;
int Nivel::glTexSuelo;
int Nivel::glTexPared;

float Nivel::guiRojo ;
float Nivel::guiVerde ;
float Nivel::guiAzul ;

void
Nivel::Inicializar()
{
    BITMAP *bitmap = NULL;
    
    bitmap = load_bitmap("GUI\\gui.bmp", NULL);
    if (!bitmap)
    {
        throw Excepcion("No se puede cargar la textura del gui");
    }
    glTexGui = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);    
    
    bitmap = load_bitmap("GUI\\nivel.bmp", NULL);
    if (!bitmap)
    {
        throw Excepcion("No se puede cargar la textura del nivel de gui");
    }
    glTexNivel = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);    

    string temp ;
    for ( int i = 0 ; i < 10 ; i++ )
    { 
        temp = "GUI\\" ;
        temp += i+'0' ;
        temp += ".bmp" ;

        bitmap = load_bitmap(temp.c_str(), NULL);
        if (!bitmap)
        {
            throw Excepcion("No se puede cargar la textura del gui de numero");
        }
        glTexNum[i] = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);                
    }
    
    bitmap = load_bitmap("GUI\\tiempo.bmp", NULL);
    if (!bitmap)
    {
        throw Excepcion("No se puede cargar la textura del nivel de gui");
    }
    glTexTiempo = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);        
    

    bitmap = load_bitmap("Texturas\\desert.bmp", NULL);
    
    if (!bitmap)
    {
        throw Excepcion("No se puedo cargar la textura desert.bmp");
    }
    
    // Creamos la textura
    glTexSuelo = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);

    bitmap = load_bitmap("Texturas\\ladrillos.bmp", NULL);
    
    if (!bitmap)
    {
        throw Excepcion("No se puedo cargar la textura ladrillos.bmp");
    }
    
    // Creamos la textura
    glTexPared = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);

    guiRojo = 0.0 ;
    guiVerde = 0.0 ;
    guiAzul = 1.0 ;
}

Nivel::Nivel(int numNivel):
    numNivel(numNivel)
{
    tiempo = numNivel * 60 ;
    string ficheroNivel = "Niveles\\Nivel" ;
    ficheroNivel += (numNivel / 10)+'0' ;
    ficheroNivel += (numNivel % 10)+'0' ;
    ficheroNivel += ".dat" ;
    fstream fic ;
    try
    {
        fic.open(ficheroNivel.c_str(), ios_base::in );
        if ( !fic.is_open() )
        {
            throw Excepcion( "No se pudo abrir el fichero de nivel " + ficheroNivel);
        }            
        
        fic >> w ;
        fic >> h ;

        altura = new int *[w] ;
        for ( int i = 0 ; i < w ; i++ )
        {
            altura[i] = new int[h] ;
        }
        
        for ( int y = 0 ; y < h ; y++ )
        {
            for ( int x = 0 ; x < w ; x++ )
            {
                fic >> altura[x][y] ;
            }
        }
        
        string temp ;        

        // Posicion de inicio del jugador
        int iniciox, inicioy ;
        
        // Posicion de salida del nivel
        int salidax, saliday ;
        
        bool salir = false;
        do
        {
            do 
            {
                if ( !fic.good() )
                {
                    salir = true ;
                }
                fic >> temp ;
            }while ( temp != "#" && !salir);
            
            if ( !salir )
            {
                fic >> temp ;
                if ( temp == "INICIO" )
                {
                    fic >> iniciox >> inicioy ;    
                }
                else if ( temp == "FINAL" )
                {
                    fic >> salidax, saliday ;
                }
                else if ( temp == "PUERTA" )
                {
                    int num, px, py, ori;
                    string orien, ini ;
                    fic >> num >> px >> py >> orien >> ini;
                    if ( orien == "N" )
                    {
                        ori = Orientacion::Norte ;
                    }
                    else if ( orien == "E" )
                    {
                        ori = Orientacion::Este ;
                    }
                    else if ( orien == "S" )
                    {
                        ori = Orientacion::Sur ;
                    }
                    else if ( orien == "O" )
                    {
                        ori = Orientacion::Oeste ;
                    }
                    else
                    {
                        throw Excepcion("Formato incorrecto");
                    }

                    Puerta *puerta = new Puerta(
                        this, 
                        num, 
                        Vector(px*escala - escala/2, altura[px-1][py-1]*escala, py*escala - escala/2),
                        Vector(escala/2, escala/2, escala/2),
                        ori);
                    //puerta->Excitar();
                    item.push_back(puerta);
                    
                    if ( ini == "A" )
                    {
                        puerta->Excitar();
                    }
                    else if ( ini != "D" )
                    {
                        throw Excepcion("Formato incorrecto");
                    }                    
                }
                else if ( temp == "ASCENSOR" ) 
                {
                    int num, px, py, des, ex;
                    fic >> num >> px >> py >> des >> ex ;

                    Ascensor *ascensor = new Ascensor(
                        this, 
                        num, 
                        Vector(px*escala - escala/2, des*escala, py*escala - escala/2),
                        Vector(escala/2, escala/2, escala/2),
                        Vector(px*escala - escala/2, des*escala, py*escala - escala/2),
                        Vector(px*escala - escala/2, ex*escala, py*escala - escala/2));                        
                    //ascensor->Excitar();
                    item.push_back(ascensor);
                }                
                else if ( temp == "LOSA" )
                {
                    int num, px, py, target, tipo ;
                    string palabraTipo, ini ;
                    fic >> num >> px >> py >> target >> palabraTipo >> ini;
                    if ( palabraTipo == "E" )
                    {
                        tipo = Losa::Excitador ;
                    }
                    else if ( palabraTipo == "D" )
                    {
                        tipo = Losa::Desexcitador ;
                    }
                    else if ( palabraTipo == "I" )
                    {
                        tipo = Losa::Intercambiador ;
                    }
                    else
                    {
                        throw Excepcion("Formato incorrecto");
                    }
                    
                    Losa *losa = new Losa(
                        this,
                        num,
                        Vector(px*escala-escala/2, altura[px-1][py-1]*escala, py*escala-escala/2),
                        Vector(escala/2, escala/2, escala/2),
                        target,
                        tipo);
                    item.push_back(losa);  
                    
                    if ( ini == "D" )
                    {
                        losa->Desexcitar();
                    }
                    else if ( ini != "A" )
                    {
                        throw Excepcion("Formato incorrecto");
                    }
                }  
                else if ( temp == "LUZ_AMBIENTE" )
                {
                    int intensidad ;
                    fic >> intensidad ;
                    
                    float ambient[4] ;
                    ambient[0] = ambient[1] = ambient[2] = intensidad/255.0 ;
                    ambient[3] = 1.0 ;
                    
                    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
                }
                else if ( temp == "LUZ" )
                {
                    int cod, x, y, r, g, b, ate ;
                    fic >> cod >> x >> y >> r >> g >> b >> ate ;
                    
                    Luz *l = new Luz(cod, Vector(x*escala-escala/2, altura[x-1][y-1]*escala+escala/2, y*escala-escala/2), r, g, b, ate);
                    luz.push_back(l);
                    
                    float luz[4] ;
                    luz[0] = r/255.0 ;
                    luz[1] = g/255.0 ;
                    luz[2] = b/255.0 ;
                    luz[3] = 1.0 ;
                    
                    float pos[4] ;
                    pos[0] = x*escala - escala/2 ;
                    pos[1] = altura[x-1][y-1]*escala + escala/2 ;
                    pos[2] = y*escala - escala/2 ;
                    pos[3] = 1.0 ;
                    
                    /*
                    float atenuacion[3] ;
                    atenuacion[0] = 1.0 ;
                    atenuacion[1] = 0.0 ;
                    atenuacion[2] = 1.0 ;
                    */
                    
                    
                    glEnable(GL_LIGHT0 + cod);
                    glLightfv(GL_LIGHT0+cod, GL_DIFFUSE, luz);
                    glLightfv(GL_LIGHT0+cod, GL_SPECULAR, luz);
                    glLightfv(GL_LIGHT0+cod, GL_POSITION, pos);
                    glLightf(GL_LIGHT0+cod, GL_QUADRATIC_ATTENUATION, 1.0);
                }
                    
            }
        }while (!salir) ;
        
        fic.close();
        
        GenerarMapa();
        
        // Creamos la camara
        camara = new Camara() ;
        
        // Creamos el jugador
        jugador = new Jugador(this);
        jugador->SetPosicion( Vector(iniciox*escala-(escala/2), altura[iniciox-1][inicioy-1]*escala+1*escala, inicioy*escala-(escala/2) ) );
        jugador->SetRadio( Vector(1.5, 2, 1.5) );
        jugador->SetEsObstaculo(true);
        
        // Linkamos la camara para que siga al jugador
        camara->Linkar(jugador);
        
        estado = EN_Jugando ;
        
        // Creamos los bloques que representan la geometría del escenario
        
        for ( int x = 0 ; x < w ; x ++ )
        {
            for ( int y = 0 ; y < h ; y ++ )
            {
                Bloque * bloque = new Bloque( this, Vector(x*escala + (escala/2), (altura[x][y]*escala)/2, y*escala + (escala/2)) );
                bloque->SetRadio( Vector(escala/2, (altura[x][y]*escala)/2, escala/2) );
                bloque->SetEsObstaculo(true);
                item.push_back( bloque );
            }
        }     
        
        visionEstereo = false ;  
        zoom = 1.0 ;
        disVista = 5 ;
    }
    catch (Excepcion &ex)
    {
        fic.close();
        throw Excepcion(ex, "Nivel::Nivel(string)");
    }
    catch (exception &ex)
    {
        fic.close();
        throw Excepcion(ex, "Nivel::Nivel(string)");
    }    
    
    frecTiempo.setFrec(1);
}

Nivel::~Nivel()
{
    try
    {
        if ( !altura || !camara || !jugador )
        {
            throw Excepcion("Error");
        }
        for ( int i = 0 ; i < w ; i++ )
        {
            if ( !altura[i] )
            {
                throw Excepcion("Error");
            }
            delete altura[i];
        }
        for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
        {
            delete (*i);
        }
        item.clear();
        
        for ( list <Luz *>::iterator i = luz.begin() ; i != luz.end() ; ++i )
        {
            delete (*i);
        }
        luz.clear();
        
        delete altura ;
        altura = NULL ;
        delete camara ;
        camara = NULL ;
        delete jugador ;
        jugador = NULL ;
    }
    catch (Excepcion &ex)
    {
        throw Excepcion(ex, "Nivel::~Nivel(string)");
    }
    catch (exception &ex)
    {
        throw Excepcion(ex, "Nivel::~Nivel(string)");
    }
}

void
Nivel::GenerarMapa()
{
    try
    {
        if ( !altura )
        {
            throw Excepcion("altura vale null");
        }
        
        callList = glGenLists(1);
        if ( !callList )
        {
            throw Excepcion("No se pueden reservar callLists");
        }
        
        
        
        float ambient[4] = {0.6,0.6,0.6,1.0};
        float specular[4] = {0.3,0.3,0.3,1.0};
        //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);        
        
        glNewList(callList, GL_COMPILE);
        
            glEnable(GL_TEXTURE_2D);

            // Generamos las baldosas del suelo
            glBindTexture(GL_TEXTURE_2D, glTexSuelo);
            glBegin(GL_QUADS);
                glColor4f(1.0, 1.0, 1.0, 1.0);
                glNormal3f(0,1,0);
                for ( int x = 0 ; x < w ; x++ )
                {
                    for ( int y = 0 ; y < h ; y++ )
                    {
                        glTexCoord2f(0, 1);
                        glVertex3f((x  ), altura[x][y], (y  ));
                        glTexCoord2f(0, 0);
                        glVertex3f((x  ), altura[x][y], (y+1));
                        glTexCoord2f(1, 0);
                        glVertex3f((x+1), altura[x][y], (y+1));
                        glTexCoord2f(1, 1);
                        glVertex3f((x+1), altura[x][y], (y  ));
                    }
                }
                glEnd();
                glBindTexture(GL_TEXTURE_2D, glTexPared);
                glBegin(GL_QUADS);
                // Generamos las paredes
                glColor4f(1,1,1,1);
                //glColor4f(0.8,0.9,0.7,1);
                for ( int x = 1 ; x < w ; x++ )
                {
                    for ( int y = 0 ; y < h ; y++ )
                    {
                        if ( altura[x][y] < altura[x-1][y] )
                        {
                            glNormal3f(1,0,0);
                        }
                        else
                        {
                            glNormal3f(-1,0,0);
                        }
                        glTexCoord2f(3, 3*0.88*altura[x-1][y]);
                        glVertex3f((x  ), altura[x-1][y], (y  ));
                        glTexCoord2f(3, 3*0.88*altura[x][y]);
                        glVertex3f((x  ), altura[x  ][y], (y  ));
                        glTexCoord2f(0, 3*0.88*altura[x][y]);
                        glVertex3f((x  ), altura[x  ][y], (y+1));
                        glTexCoord2f(0, 3*0.88*altura[x-1][y]);
                        glVertex3f((x  ), altura[x-1][y], (y+1));                        
                        
                    }
                }
                
                for ( int x = 0 ; x < w ; x++ )
                {
                    for ( int y = 1 ; y < h ; y++ )
                    {
                        if ( altura[x][y] < altura[x][y-1] )
                        {
                            glNormal3f(0,0,1);
                        }
                        else
                        {
                            glNormal3f(0,0,-1);
                        }
                        glTexCoord2f(0, 3*0.88*altura[x][y-1]);
                        glVertex3f((x  ), altura[x][y-1], (y  ));
                        glTexCoord2f(0, 3*0.88*altura[x][y]);
                        glVertex3f((x  ), altura[x][y],   (y  ));
                        glTexCoord2f(3, 3*0.88*altura[x][y]);
                        glVertex3f((x+1), altura[x][y],   (y  ));
                        glTexCoord2f(3, 3*0.88*altura[x][y-1]);
                        glVertex3f((x+1), altura[x][y-1], (y  ));
                    }
                }
                
                // Generamos los bordes
                for ( int x = 0 ; x < w ; x++ )
                {
                    for ( int y = 0 ; y < h ; y++ )
                    {
                        glVertex3f((x  ), altura[x][0], 0);
                        glVertex3f((x  ), 0, 0);
                        glVertex3f((x+1), 0, 0);
                        glVertex3f((x+1), altura[x][0], 0);
                        
                        glVertex3f((x  ), altura[x][h-1], h);
                        glVertex3f((x  ), 0, h);
                        glVertex3f((x+1), 0, h);
                        glVertex3f((x+1), altura[x][h-1], h); 
                        
                        glVertex3f(0, altura[0][y], (y));
                        glVertex3f(0, 0, (y));
                        glVertex3f(0, 0, (y+1));
                        glVertex3f(0, altura[0][y], (y+1));
                        
                        glVertex3f((w), altura[w-1][y], (y));
                        glVertex3f((w), 0, (y));
                        glVertex3f((w), 0, (y+1));
                        glVertex3f((w), altura[w-1][y], (y+1));                                               
                    }
                } 
            glEnd();        
        glEndList();
    }
    catch (Excepcion &ex)
    {
        throw Excepcion(ex, "Nivel::GenerarMapa()");
    }
    catch (exception &ex)
    {
        throw Excepcion(ex, "Nivel::GenerarMapa()");
    }       
}

void
Nivel::DesplazarItem(Vector pos, Vector &mov, Vector rad)
{
    Vector movX = Vector (mov.GetX(), 0, 0);
    Vector movY = Vector (0, mov.GetY(), 0);
    Vector movZ = Vector (0, 0, mov.GetZ());

    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        (*i)->Colision(pos, movX, rad) ;
    }

    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        (*i)->Colision(pos+movX, movZ, rad) ;
    }
    
    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        (*i)->Colision(pos+movX+movZ, movY, rad) ;
    }
    
    mov.SetX(movX.GetX());
    mov.SetY(movY.GetY());
    mov.SetZ(movZ.GetZ());
}

void
Nivel::EmpujarItems(Vector pos, Vector mov, Vector rad)
{
    Vector movY = Vector(0, mov.GetY(), 0);
    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        (*i)->Empujar(pos, movY, rad) ;
    }
    jugador->Empujar(pos, movY, rad);
}

void
Nivel::Actualizar()
{
    if ( key[KEY_E] )
    {
        visionEstereo = true ;
    }
    if ( key[KEY_M] )
    {
        visionEstereo = false ;
    }
    
    if ( key[KEY_Z] )
    {
        zoom += 0.01 ;
    }
    if ( key[KEY_X] )
    {
        zoom -= 0.01 ;
    }
    if ( key[KEY_A] )
    {
        disVista -= 0.05 ;
    }
    if ( key[KEY_Q] )
    {
        disVista += 0.05 ;
    }
    
    // Gui
    if ( key[KEY_T] ) guiRojo += 0.01 ;
    if ( key[KEY_G] ) guiRojo -= 0.01 ;    
    if ( key[KEY_Y] ) guiVerde += 0.01 ;
    if ( key[KEY_H] ) guiVerde -= 0.01 ;    
    if ( key[KEY_U] ) guiAzul += 0.01 ;
    if ( key[KEY_J] ) guiAzul -= 0.01 ;    
    
    if ( zoom > 3.0 ) zoom = 3.0 ;
    if ( zoom < 0.1 ) zoom = 0.1 ;
    if ( disVista < 1.0 ) disVista = 1.0 ;
    if ( disVista > 50.0 ) disVista = 50.0 ;
    if ( guiRojo < 0.0 ) guiRojo = 0.0 ;
    if ( guiRojo > 1.0 ) guiRojo = 1.0 ;
    if ( guiVerde < 0.0 ) guiVerde = 0.0 ;
    if ( guiVerde > 1.0 ) guiVerde = 1.0 ;
    if ( guiAzul < 0.0 ) guiAzul = 0.0 ;
    if ( guiAzul > 1.0 ) guiAzul = 1.0 ;
    
    //Actualizar tiempo
    if (frecTiempo.comprobar())
    {
      if (tiempo>0) tiempo--;
    }


    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        (*i)->Actualizar();
    }
    
    jugador->Actualizar();
    
    if ( jugador->GetPosicion().GetY() >= 16*escala-0.1 )
    {
        estado = EN_Superado ;
    }

    if (tiempo<=0) estado = EN_Muerto;
}    

void
Nivel::Pintar()
{
    try
    {
        static float giro = 0 ;
        giro += 0.2 ;
        if ( visionEstereo )
        {
    
            glViewport(200,200,190,190);
    
            glEnable(GL_DEPTH_TEST);
            
            // Situamos la camara
            camara->Pintar(0);
            
            // Pintamos las luces
            for ( list <Luz *>::iterator i = luz.begin() ; i != luz.end() ; ++i )
            {
                (*i)->Pintar();
            }
            
            // Escalamos el escenario
            glPushMatrix();
            glScalef(escala,escala,escala);
            
            // Pintamos el escenario
            glCallList(callList);
            
            // Los items no es necesario escalarlos
            glPopMatrix();
            
            // Pintamos los items
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Normal);
            }
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Transparente);
            }
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::SiempreVisible);
            }    
            
            
            //----------------------------
            glViewport(400,200,190,190);
    
            glEnable(GL_DEPTH_TEST);
            
            // Situamos la camara
            camara->Pintar(1);
            
            // Pintamos las luces
            for ( list <Luz *>::iterator i = luz.begin() ; i != luz.end() ; ++i )
            {
                (*i)->Pintar();
            }
            
            // Escalamos el escenario
            glPushMatrix();
            glScalef(escala,escala,escala);
            
            // Pintamos el escenario
            glCallList(callList);
            
            // Los items no es necesario escalarlos
            glPopMatrix();
            
            // Pintamos los items
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Normal);
            }
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Transparente);
            }
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::SiempreVisible);
            }            
            
            // Pintamos el GUI
            //PintarGui(); 
            
            // Pintamos la vista
        }
        else
        {
            glViewport(0,0,SCREEN_W,SCREEN_H);
        
            glEnable(GL_DEPTH_TEST);
            
            // Situamos la camara
            camara->Pintar();
            
            // Pintamos las luces
            for ( list <Luz *>::iterator i = luz.begin() ; i != luz.end() ; ++i )
            {
                (*i)->Pintar();
            }
            
            // Escalamos el escenario
            glPushMatrix();
            glScalef(escala,escala,escala);
            
            // Pintamos el escenario
            glCallList(callList);
            
            // Los items no es necesario escalarlos
            glPopMatrix();
            
            // Pintamos los items
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Normal);
            }           
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Transparente);
            }
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::SiempreVisible);
            }            
            
            // Pintamos el GUI
            PintarGui(); 
            
            // Pintamos la vista
            
            // Borramos el depth buffer para poder pintar por encima la minivista
            // Es importante que la minivista sea lo ultimo ya que tras esto hemos perdido
            // la informacion del depth buffer
            glClear(GL_DEPTH_BUFFER_BIT);
            
            glViewport(SCREEN_W-95,SCREEN_H-95,90,90);
        
            glEnable(GL_DEPTH_TEST);
            
            // Situamos la camara
            glLoadIdentity();


            glRotatef(45,1,0,0);
            glTranslatef(0,0,-disVista);
            glScalef(zoom*0.1,zoom*0.1,zoom*0.1);
            glRotatef(giro,0,1,0);
            glTranslatef(-(w*escala)/2, -120, -(h*escala)/2);
            
            // Pintamos las luces
            for ( list <Luz *>::iterator i = luz.begin() ; i != luz.end() ; ++i )
            {
                (*i)->Pintar();
            }
            
            // Escalamos el escenario
            glPushMatrix();
            glScalef(escala,escala,escala);
            
            // Pintamos el escenario
            glCallList(callList);
            
            // Los items no es necesario escalarlos
            glPopMatrix();
            
            // Pintamos los items
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Normal);
            }           
            jugador->Pintar(EtapaRender::Normal);
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::Transparente);
            }
            for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
            {
                (*i)->Pintar(EtapaRender::SiempreVisible);
            }

        }
    }
    catch (Excepcion &ex)
    {
        throw Excepcion(ex, "Nivel::Pintar()");
    }
    catch (exception &ex)
    {
        throw Excepcion(ex, "Nivel::Pintar()");
    }       
}


void 
Nivel::PintarGui()
{
    allegro_gl_set_projection();
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
 
        glColor4f(guiRojo, guiVerde, guiAzul, 0.3);
        
        // Fondo del gui
        glBindTexture(GL_TEXTURE_2D, glTexGui);        
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1);
            glVertex2f(0, 0);
            glTexCoord2f(1, 1);
            glVertex2f(SCREEN_W, 0);
            glTexCoord2f(1, 0);
            glVertex2f(SCREEN_W, 100);
            glTexCoord2f(0, 0);
            glVertex2f(0, 100);
        glEnd();
        
        // Palabra nivel
        glBindTexture(GL_TEXTURE_2D, glTexNivel);        
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1);
            glVertex2f(11, 3);
            glTexCoord2f(1, 1);
            glVertex2f(85, 3);
            glTexCoord2f(1, 0);
            glVertex2f(85, 32);
            glTexCoord2f(0, 0);
            glVertex2f(11, 32);
        glEnd();
        
        // Numero de nivel
        if ( numNivel / 10 > 0 )
        {
            glBindTexture(GL_TEXTURE_2D, glTexNum[numNivel/10]);        
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1);
                glVertex2f(87, 3);
                glTexCoord2f(1, 1);
                glVertex2f(109, 3);
                glTexCoord2f(1, 0);
                glVertex2f(109, 32);
                glTexCoord2f(0, 0);
                glVertex2f(87, 32);
            glEnd();                
        }
        
        glBindTexture(GL_TEXTURE_2D, glTexNum[numNivel%10]);        
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1);
            glVertex2f(100, 3);
            glTexCoord2f(1, 1);
            glVertex2f(122, 3);
            glTexCoord2f(1, 0);
            glVertex2f(122, 32);
            glTexCoord2f(0, 0);
            glVertex2f(100, 32);
        glEnd();  
        
        // Palabra tiempo
        glBindTexture(GL_TEXTURE_2D, glTexTiempo);        
        glBegin(GL_QUADS);
            glTexCoord2f(0, 1);
            glVertex2f(150, 3);
            glTexCoord2f(1, 1);
            glVertex2f(249, 3);
            glTexCoord2f(1, 0);
            glVertex2f(249, 32);
            glTexCoord2f(0, 0);
            glVertex2f(150, 32);
        glEnd();   
        
        // Tiempo restante
        int tempTiempo = tiempo ;
        int x = 300 ;
        do
        {
            glBindTexture(GL_TEXTURE_2D, glTexNum[tempTiempo%10]);        
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1);
                glVertex2f(x, 3);
                glTexCoord2f(1, 1);
                glVertex2f(x+22, 3);
                glTexCoord2f(1, 0);
                glVertex2f(x+22, 32);
                glTexCoord2f(0, 0);
                glVertex2f(x, 32);
            glEnd();
            
            x -= 12 ;  
            tempTiempo /= 10 ;
        }while ( tempTiempo > 0 ) ;
        

        glColor4f(1,1,1,1);
        
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);
    allegro_gl_unset_projection();
}

void
Nivel::GetPosicionJugador(int &x, int &z)
{
    x = (int)(jugador->GetPosicion().GetX() / escala );
    z = (int)(jugador->GetPosicion().GetZ() / escala );    
}

float
Nivel::GetAlturaJugador()
{
    return jugador->GetPosicion().GetY()-jugador->GetRadio().GetY() ;
}

Ascensor *
Nivel::GetAscensorEn(float px, float pz)
{
    int x, y;
    x = px / escala ;
    y = pz / escala ;

    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        if ( (*i)->EsAscensor() )
        {
            if ( ((int)((*i)->GetPosicion().GetX() / escala)) == x &&
                 ((int)((*i)->GetPosicion().GetZ() / escala)) == y )
            {
                return (Ascensor *)(*i) ;
            }
        }
    }
    
    return NULL;
}

void
Nivel::ExcitarCodigo(int codigo)
{
    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        if ( (*i)->GetCodigo() == codigo )
        {
            (*i)->Excitar();
        }
    }
}

void
Nivel::DesexcitarCodigo(int codigo)
{
    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        if ( (*i)->GetCodigo() == codigo )
        {
            (*i)->Desexcitar();
        }
    }
}

void
Nivel::IntercambiarCodigo(int codigo)
{
    for ( list <Item *>::iterator i = item.begin() ; i != item.end() ; ++i )
    {
        if ( (*i)->GetCodigo() == codigo )
        {
            (*i)->Intercambiar();
        }
    }
}
