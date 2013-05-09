    class SDLApp: public SDLInput

        Init (  unsigned int sdl_flags = SDL_INIT_EVERYTHING,
                unsigned int img_flags = IMG_INIT_PNG );
        bool IsRunning ( void );
        bool IsFullScreen ( void );
        void SetGameState ( bool state );
        void handleInput ( void );
        //void onExit();
        //void Run();
        //void Update();
        //void Draw();

        void setTitle ( std::string app_name );
        bool setIcon (  std::string app_icon, /*SDL_Color colorkey,*/
                        unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

        SDL_Surface *screen; // primary (think: visible) video memory
        SDL_Event input; // input events; keyboard, mouse
        FPS fps; // timer for tracking frames per second
        bool show_fps; // fps counter
        bool fullscreen; // toggle window & fullscreen states
        bool game_state; // global app state

    class SDLDisplay

        bool setVideoMode ( unsigned int screen_width = 320,
                            unsigned int screen_height = 240,
                            unsigned int screen_bpp = 8,
                            unsigned int video_flags = 0 );
        updateScreen();

        static bool setTransparency ( SDL_Surface *video_buffer,
                                  unsigned int r, unsigned int g, unsigned int b,
                                  unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

        static bool DrawSurface (   SDL_Surface *dest_buffer, SDL_Surface *source_buffer, unsigned int x, unsigned int y,
                                    SDL_Rect *offsets = NULL );

        static SDL_Surface *LoadImage ( std::string filename, /*SDL_Color colorkey,*/
                                        unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );

    class SDLDrawable
        std::vector<SDLDrawable*>Surfaces
        Update() = 0;
        Draw() = 0;
    class SDLDisplay: public SDLDrawable
        Update();
        Draw();
    class Canvas: public SDLDrawable
        Update();
        Draw();
    class Image: public SDLDrawable
        Update();
        Draw();

    class Sprite: public SDLDrawable
        std::map<SDLDrawable*, filename >sprite;
        Update();
        Draw();

    class Interface
        void ShowFPS ( void );
        void interface_GameOver ( void );
        Font timer_text; // fps timer font
        Font message_text;

    class SDLGfx
        bool DrawRectangle (  unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height,
                          unsigned int r, unsigned int g, unsigned int b );
        void DrawLine();
        void DrawCircle();
        void DrawPIxel();

---

    class AppState
        Resume() = 0;
        Pause() = 0;
        HandleEvents() = 0;
        Update() = 0;
        Draw() = 0;
        SetState();

    class TTcards: public AppState
        unsigned int get_turn ( void );
        void player_turn ( unsigned int player );
        void moveTo ( unsigned int x, unsigned int y );
        Board board; // game board
        CardDebug debug; // debug support for card attributes
        Collection collection; // cards database
        CardRules rules;
        CPUPlayer AI;
        CardView card; // card rendering
        CardHand hand[2]; // player hand
        Player player[2]; // player on board
        Sprite cursor; // interface cursor
        Audio mixer1, mixer2; // Two audio mixing channels for playing sound effects
        Audio music; // holds our musical tracks
        std::pair <int, int> coords; // x, y coords

