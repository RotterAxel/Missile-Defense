//Programmer: Axel Rotter Morgado
//Date: 18.04.2016

#include "GameApp.h"

GameApp::GameApp()
{
	//Reset all required values
	m_hWnd		= NULL;
	m_hInstance = NULL;
	m_hSpriteDC = NULL;
}

GameApp::~GameApp()
{
	ShutDown();
}

//--------------------------------------------------------------------------
//Name: InitInstance()
//Desc: Here we ger our main instance handle, initialize the sprites to the 
//		correct resource and define the gameboard boundaries.
//--------------------------------------------------------------------------
bool GameApp::InitInstance(HINSTANCE hInstance, LPCTSTR lpCmdLine, int iCmdShow)
{
	if (!CreateDisplay()) { ShutDown(); return false; }

	m_hInstance = hInstance;

	//Game starts paused
	m_Paused = true;

	//Initialize Sprites
	InitializeSprites();

	//Initialize the bounding rectangles
	mBoardBounds = Rect(0, 0, 864, 504);

	return true;
}

//--------------------------------------------------------------------------
//Name: CreateDisplay()
//Desc: This is the standard window creation procedure to create a window 
//		class and to register the newly created window with Windows. In this 
//		case the window cannot be resized and will be "spawned" at a location
//		that Windows sees fit. 
//--------------------------------------------------------------------------
bool GameApp::CreateDisplay()
{
	LPTSTR WindowTitle = _T("2D Engine");
	m_WndCaption = *WindowTitle;

	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wc.hIcon = ::LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = WindowTitle;

	RegisterClass(&wc);

	//WS_OVERLAPPED | WS_SYSMENU: Window cannot be resized
	//and does not have a min/max button.
	m_hWnd = CreateWindow(WindowTitle, WindowTitle, WS_OVERLAPPED
		| WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT, m_cWindowWidth,
		m_cWindowHeight, NULL, LoadMenu(wc.hInstance, MAKEINTRESOURCE(IDR_MENU)),
		wc.hInstance, this);

	if (!m_hWnd) return false;

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}

//--------------------------------------------------------------------------
//Name: BeginGame()
//Desc: This is the main message pump, it captures the appropriate messages,  
//		and routes them throught the applciation class for which it was 
//		intended, therefore giving full class access.
//--------------------------------------------------------------------------
int GameApp::BeginGame()
{
	MSG msg;
	
	//Start main loop
	while (1)
	{
		//Windows message goes through here
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//Application message triggers the FrameAdvance() function
		else
		{
			FrameAdvance();
		}
	}

	return 0;
}

//--------------------------------------------------------------------------
//Name: ShutDown()
//Desc: Shuts down the application and sets the window handle free.
//--------------------------------------------------------------------------
bool GameApp::ShutDown()
{
	if (m_hWnd) DestroyWindow(m_hWnd);

	m_hWnd = NULL;

	return true;
}

//--------------------------------------------------------------------------
//Name: StaticWndProc () (Static CallBack)
//Desc: This is the main message pump for ALL display devices, it captures
//		the appropriate messages, and routes them throught the applciation 
//		class for which it was intended, therefore giving full class access.
//Note:	It is VITALLY important that you should pass your 'this' pointer to
//		the lParam parameter of the CreateWindow function if you wish to be
//		able to pass messages back to that app object.
//--------------------------------------------------------------------------
LRESULT CALLBACK GameApp::StaticWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	//If this is a create message, trap the 'this' pointer passed in and store it within the window.
	if (Message == WM_CREATE) SetWindowLong(hWnd, GWL_USERDATA, (LONG)((CREATESTRUCT FAR *)lParam)->lpCreateParams);

	//Obtain the correct destination for this message
	GameApp *Destination = (GameApp*)GetWindowLong(hWnd, GWL_USERDATA);

	//If the hWnd has a related class, pass it through
	if (Destination) return Destination->DisplayWndProc(hWnd, Message, wParam, lParam);

	//No destination found Default window Procedure
	return DefWindowProc(hWnd, Message, wParam, lParam);
}

//--------------------------------------------------------------------------
//Name: DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam) 
//Desc: Windows message catcher that reacts to keyboard and mouse input.
//--------------------------------------------------------------------------
LRESULT GameApp::DisplayWndProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	
	switch (Message)
	{
	//Set the initial values for the game state
	case WM_CREATE:

		//Application values
		m_PlayerScore = 0;
		m_PlayerLives = 3;
		m_GameOver = false;
		mEnemySpawnTimer = new SpawnTimer();
		mEnemyMissile = new EnemyMissile();
		gEnemyMissiles.push_back(*mEnemyMissile);

		//System memory DCs
		m_oBackBuffer = new BackBuffer(hWnd, m_cClientWidth, 
			m_cClientHeight);
		m_hSpriteDC = CreateCompatibleDC(0);
		break;

	
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	case WM_LBUTTONDOWN:
		mPlayerMissile = new PlayerMissile(ScreenToClientCoords());
		gPlayerMissiles.push_back(*mPlayerMissile);
		break;

	case WM_RBUTTONDOWN:
		Unpause();
		break;

	case WM_KEYDOWN:
		// Which key was pressed?
		switch (wParam)
		{
		case VK_ESCAPE:
			Pause();
			return 0;
			break;

		case VK_SPACE:
			if (m_GameOver)
			{
				m_GameOver = false;
				m_PlayerScore = 0;
			}
			break;
		} // End Switch

		break;

	//Clean up all game elements used
	case WM_DESTROY:
		delete mGameBoard;
		delete mSilo;
		delete mReticle;
		delete mPlayerMissileSprite;
		delete mEnemyMissileSprite;
		delete mPauseScreen;
		delete mGameOverSprite;

		delete mScore;
		delete mLives;
		delete mHeart;
		
		delete mPlayerMissile;
		delete mDetonation;
		delete mEnemyMissile;
		delete mEnemySpawnTimer;

		for (int i = 9; i >= 0; --i)
		{
			delete mDetonationArray[i];
			mDetonationArray[i] = 0;
		}
		
		for (int i = 9; i >= 0; --i)
		{
			delete mPlayerOnes[i];
			mPlayerOnes[i] = 0;
		}

		for (int i = 9; i >= 0; --i)
		{
			delete mPlayerDecimals[i];
			mPlayerDecimals[i] = 0;
		}

		for (int i = 9; i >= 0; --i)
		{
			delete mPlayerHundreds[i];
			mPlayerHundreds[i] = 0;
		}

		DeleteDC(m_hSpriteDC);
		PostQuitMessage(0);
		break;

	case WM_COMMAND:

		// Process Menu Items
		switch (LOWORD(wParam))
		{
		case ID_FILE_EXIT:
			// Recieved key/menu command to exit app
			SendMessage(m_hWnd, WM_CLOSE, 0, 0);
			return 0;
		}

		default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	

	return 0;
}

//--------------------------------------------------------------------------
//Name: FrameAdvance() 
//Desc: Updates the timer and sets it to 60 FPS. Updates sprite logic and   
//		position.Calls the Draw() function, calls the BackBuffer class and  
//		if the game is over calls the ResetGameState() function.
//--------------------------------------------------------------------------

void GameApp::FrameAdvance()
{
	//Update the timer to the next frame at 60FPS
	m_oTimer.Tick(60.0f);

	//Update all sprites
	Update();

	//Draws the objects on the client area
	Draw();
	m_oBackBuffer->present();
	
	//If gameover reset the games parameters
	if (m_GameOver)
	{
		Pause();
		ResetGameState();
	}
	
}

//--------------------------------------------------------------------------
//Name: FrameAdvance() 
//Desc: If the game is paused, but the game is not over, we see the "game 
//		paused" screen. If the game is both paused and over we see the 
//		game over screen. Else the game draws the game components at the 
//		positions set in the logic.
//--------------------------------------------------------------------------
void GameApp::Draw()
{
	//If paused and not game over then draw "Pause Screen"
	if (m_Paused == true && m_GameOver == false)
	{
		mGameBoard->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPauseScreen->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mSilo->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mReticle->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		DrawPlayerScore();
		DrawPlayerLives();
	} //End if statement

	 //If paused and game over then draw "Game Over Screen"
	else if (m_Paused == true && m_GameOver == true)
	{
		mGameBoard->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		DrawPlayerScore();
		DrawPlayerLives();
		mGameOverSprite->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
	} //End else if statement

	//If not paused and not game over then draw game elements at updated 
	//positions
	else
	{
		mGameBoard->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mSilo->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mReticle->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		DrawPlayerScore();
		DrawPlayerLives();

		//Iterate through player missiles and draw at each position
		std::list<PlayerMissile>::iterator i = gPlayerMissiles.begin();
		while (i != gPlayerMissiles.end())
		{
			mPlayerMissileSprite->mPosition = i->GetCurrPos();
			mPlayerMissileSprite->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
			++i;
		} //End list<PlayerMissile> while

		//Iterate through enemy missiles and iterate through each position
		std::list<EnemyMissile>::iterator k = gEnemyMissiles.begin();
		while (k != gEnemyMissiles.end())
		{
			mEnemyMissileSprite->mPosition = k->GetCurrPos();
			mEnemyMissileSprite->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
			++k;
		} //End list<EnemyMissile> while

		//Iterate through each of the detonations on screen, extract the frame of
		//detonation each detonation is on and draw that detonation frame.
		std::list<Detonation>::iterator j = gMissileDet.begin();
		while (j != gMissileDet.end())
		{
			int counter = j->animationFrame;
			float temp = m_oTimer.GetTimeElapsed();
			j->timeToNextFrame += temp;

			if (j->animationFrame > 9)
			{
				j = gMissileDet.erase(j);
			}
			else
			{
				mDetonationArray[counter]->mPosition = j->detonationPoint;
				mDetonationArray[counter]->mBoundingCircle.c = j->detonationPoint;
				mDetonationArray[counter]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

				if (j->timeToNextFrame >= j->frameTime)
				{
					++j->animationFrame;
					j->timeToNextFrame = 0.0f;
				}
				++j;
			}
		} //End list<Detonation> while

		
	} //End else statement
}

//--------------------------------------------------------------------------
//Name: Update() 
//Desc: Update the logic of each game element when the game is not paused.
//--------------------------------------------------------------------------
void GameApp::Update()
{
	if (!m_Paused)
	{
		UpdateReticle();
		UpdateMissiles();
		UpdateDetonation();
		UpdateEnemyMissiles();
	}
}

//--------------------------------------------------------------------------
//Name: Pause() 
//Desc: Pause the game and show the cursor.
//--------------------------------------------------------------------------
void GameApp::Pause()
{
	m_Paused = true;

	//Game is paused-- release capture on mouse
	ReleaseCapture();

	//Show the cursor
	ShowCursor(true);
}


//--------------------------------------------------------------------------
//Name: Unpause() 
//Desc: Unpause the game. We get the coordinates of the reticle on the cleint
//		area and convert them to screen coorinates, set the mouse to that 
//		coordinate and update the last mouse position to the position the 
//		mouse is in after the translation to the new coordinates.
//--------------------------------------------------------------------------
void GameApp::Unpause()
{
	POINT p = mReticle->mPosition;

	//Convert the coordinates of the reticle on the client area to window 
	//coordinates.
	ClientToScreen(m_hWnd, &p);

	//When unpaused set the cursor to the position of the aiming reticle and
	//store cursor position to the last position it was in. 
	SetCursorPos(p.x, p.y);
	GetCursorPos(&mLastMousePos);

	//Set Paused to False
	m_Paused = false;

	//Capture the mouse
	SetCapture(m_hWnd);

	//Hide the cursor when not paused
	ShowCursor(false);
}

//--------------------------------------------------------------------------
//Name: ScreenToClientCoords() 
//Desc: Method that converts the screen coordinates to client coordinates,
//		stores the client coordinate in the mCurrMousePos data member and	
//		returns a Vec2 coordinate. 
//--------------------------------------------------------------------------
Vec2 GameApp::ScreenToClientCoords()
{
	Vec2 mousePos;
	LPPOINT mousePosition = 0;

	//Getting and storing current mouse coodinates on screen. We convert the
	//data member to an LPPOINT by storing a dereferenced POINT in an LPPOINT
	GetCursorPos(&mCurrMousePos);
	mousePosition = &mCurrMousePos;

	//Convert the screen coordinates to client coordinates
	ScreenToClient(m_hWnd, mousePosition);

	//Stores the new coordinates 
	mCurrMousePos = *mousePosition;
	mousePos = mCurrMousePos;

	return mousePos;
}

//--------------------------------------------------------------------------
//Name: InitializeSprites() 
//Desc: Method where all Sprites are initialized to their corresponding 
//		resource.
//--------------------------------------------------------------------------
void GameApp::InitializeSprites()
{
	//Place holders for the sprites constructors datamembers.
	Circle	boundingCircle;
	Vec2	position0 = m_cClientCenter;
	Vec2	velocity0(0.0f, 0.0f);

	//Game Board
	mGameBoard = new Sprite(m_hInstance, IDB_GAMEBOARD, IDB_GAMEBOARDMASK,
		boundingCircle, position0, velocity0);

	//PauseScreen
	mPauseScreen = new Sprite(m_hInstance, IDB_PAUSE_SCREEN, IDB_PAUSE_SCREEN_MASK,
		boundingCircle, position0, velocity0);


	//Game Over Screen
	mGameOverSprite = new Sprite(m_hInstance, IDB_GAMEOVER, IDB_GAMEOVERMASK,
		boundingCircle, position0, velocity0);

	position0.x = 432;
	position0.y = 500;

	//The main character
	mSilo = new Sprite(m_hInstance, IDB_SILO, IDB_SILOMASK,
		boundingCircle, position0, velocity0);

	position0.x = 432;
	position0.y = 252;
	boundingCircle.c = position0;
	boundingCircle.r = 8;

	//Recticle, we define a bounding circle because we will use these dimensions
	//throughout the entire program
	mReticle = new Sprite(m_hInstance, IDB_RETICLE, IDB_RETICLEMASK,
		boundingCircle, position0, velocity0);

	position0.x = 432;
	position0.y = 498;
	boundingCircle.c = position0;
	boundingCircle.r = 1;
	//Sprites for the players missile
	mPlayerMissileSprite = new Sprite(m_hInstance, IDB_MISSILE, IDB_MISSILEMASK,
		boundingCircle, position0, velocity0);

	position0.x = 450;
	position0.y = 482;
	boundingCircle.c = position0;
	boundingCircle.r = 1;
	//Sprites for the enemy missiles
	mEnemyMissileSprite = new Sprite(m_hInstance, IDB_ENEMYMISSILE, IDB_ENEMYMISSILEMASK,
		boundingCircle, position0, velocity0);

	//Sprites for the explosion animation
	position0.x = 200;
	position0.y = 200;
	boundingCircle.c = position0;
	boundingCircle.r = 2;
	mDetonationArray[0] = new Sprite(m_hInstance, IDB_EXPLOSION1, IDB_EXPLOSIONMASK1,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 4;
	mDetonationArray[1] = new Sprite(m_hInstance, IDB_EXPLOSION2, IDB_EXPLOSIONMASK2,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 6;
	mDetonationArray[2] = new Sprite(m_hInstance, IDB_EXPLOSION3, IDB_EXPLOSIONMASK3,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 8;
	mDetonationArray[3] = new Sprite(m_hInstance, IDB_EXPLOSION4, IDB_EXPLOSIONMASK4,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 11;
	mDetonationArray[4] = new Sprite(m_hInstance, IDB_EXPLOSION5, IDB_EXPLOSIONMASK5,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 13;
	mDetonationArray[5] = new Sprite(m_hInstance, IDB_EXPLOSION6, IDB_EXPLOSIONMASK6,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 15;
	mDetonationArray[6] = new Sprite(m_hInstance, IDB_EXPLOSION7, IDB_EXPLOSIONMASK7,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 18;
	mDetonationArray[7] = new Sprite(m_hInstance, IDB_EXPLOSION8, IDB_EXPLOSIONMASK8,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 20;
	mDetonationArray[8] = new Sprite(m_hInstance, IDB_EXPLOSION9, IDB_EXPLOSIONMASK9,
		boundingCircle, position0, velocity0);

	boundingCircle.r = 24;
	mDetonationArray[9] = new Sprite(m_hInstance, IDB_EXPLOSION10, IDB_EXPLOSIONMASK10,
		boundingCircle, position0, velocity0);

	//Score Numbers
	LoadScoreSprites(m_hInstance);

	//Load player life bar
	position0.x = 726;
	position0.y = 9;

	//Lives text
	mLives = new Sprite(m_hInstance, IDB_LIVES, IDB_LIVES_MASK,
		boundingCircle, position0, velocity0);
	
	//Hearts bitmaps
	mHeart = new Sprite(m_hInstance, IDB_HEART, IDB_HEARTMASK,
		boundingCircle, position0, velocity0);

}

//--------------------------------------------------------------------------
//Name: ResetGameState() 
//Desc: Reset the players lives, clear the lists and reset the enemy missile
//		data members.
//--------------------------------------------------------------------------
void GameApp::ResetGameState()
{
	m_PlayerLives = 3;

	gMissileDet.clear();
	gPlayerMissiles.clear();
	gEnemyMissiles.clear();

	mEnemyMissile = new EnemyMissile();
	gEnemyMissiles.push_back(*mEnemyMissile);
	mEnemySpawnTimer->Spawn();
}

//--------------------------------------------------------------------------
//Name: UpdateReticle() 
//Desc: Calculate the mouses velocity, get the time that has elapsed since 
//		the last frame, update the velocity and the position of the reticle
//		sprite, fix mouse cursor to the reticle and force the mouse and 
//		reticle inside the client area.
//--------------------------------------------------------------------------
void GameApp::UpdateReticle()
{
	GetCursorPos(&mCurrMousePos);

	//Change mouse position
	int dx = mCurrMousePos.x - mLastMousePos.x;
	int dy = mCurrMousePos.y - mLastMousePos.y;

	Vec2 dp((float)dx, (float)dy);
	float dt = m_oTimer.GetTimeElapsed();

	mReticle->mVelocity = dp / dt;
	mReticle->update(dt);
	
	//Current position is now the last position
	mLastMousePos = mReticle->mPosition;

	//Keep mouse cursor fixed to reticle
	ClientToScreen(m_hWnd, &mLastMousePos);
	SetCursorPos(mLastMousePos.x, mLastMousePos.y);

	//If the reticle hits the wall force inside
	mBoardBounds.forceInside(mReticle->mBoundingCircle);
	mReticle->mPosition = mReticle->mBoundingCircle.c;
	
}

//--------------------------------------------------------------------------
//Name: UpdateMissiles() 
//Desc: Iterate through the player missile list, check if the missile has
//		detonated, if it has detonated add it to the detonation list and 
//		if it has not detonated update its position.
//--------------------------------------------------------------------------
void GameApp::UpdateMissiles()
{
	std::list<PlayerMissile>::iterator i = gPlayerMissiles.begin();
	
	while (i != gPlayerMissiles.end())
	{
		//If the missile has detonated (reached its ending coordinate) add
		//it to the detonation list and rease that missile from the player
		//missile list.
		if (i->CheckDetonation())
		{
			Detonate(i->GetEndPos());
			i = gPlayerMissiles.erase(i);
		}
		//Update the missiles position.
		else
		{
			
			i->UpdatePosition(m_oTimer.GetTimeElapsed());
			++i;
		}
	}
}

//--------------------------------------------------------------------------
//Name: UpdateEnemyMissiles() 
//Desc: Update the spawn timer. If the spawn timer reaches 0, then spawn a 
//		missile. 
//		Iterate through the enemy missile list, check if the missile has
//		detonated, if it has detonated add it to the detonation list and 
//		if it has not detonated update its position. 
//--------------------------------------------------------------------------
void GameApp::UpdateEnemyMissiles()
{
	mEnemySpawnTimer->UpdateSpawnTime(m_oTimer.GetTimeElapsed());

		if (mEnemySpawnTimer->Spawn())
		{
			CreateEnemyMissile();
		}

	std::list<EnemyMissile>::iterator i = gEnemyMissiles.begin();
	while (i != gEnemyMissiles.end())
	{
		//If the missile has detonated (reached its ending coordinate or is 
		//inside a player detonation circle) add it to the detonation list and 
		//erase that missile from the player
		//missile list
		if (i->CheckDetonation())
		{
			--m_PlayerLives;
			Detonate(i->GetCurrPos());
			i = gEnemyMissiles.erase(i);
		}
		//Update the missiles position.
		else
		{
			i->UpdatePosition(m_oTimer.GetTimeElapsed());
			++i;
		}
	}

}

//--------------------------------------------------------------------------
//Name: UpdateDetonation() 
//Desc:	This method iterates through the detonation list and retrieves their
//		size from their corresponding sprite. It then checks if any enemy
//		missiles are inside one of the detonations boudning circles and 
//		detonates them, if not it continues the iteration.
//--------------------------------------------------------------------------
void GameApp::UpdateDetonation()
{
	std::list<Detonation>::iterator j = gMissileDet.begin();
	std::list<EnemyMissile>::iterator i = gEnemyMissiles.begin();

	while (j != gMissileDet.end())
	{
		//Get the animation frame of the detonation.
		int counter = j->animationFrame;
		//Store the size of the bounding circle of the sprite that represents
		//that animation frame.
		mDetonationArray[counter]->mBoundingCircle.c = j->detonationPoint;

		while (i != gEnemyMissiles.end())
		{
			//Iterate through all enemy missiles and check if any of them are 
			//inside the current detonation.
			if (mDetonationArray[counter]->mBoundingCircle.isInside(i->GetCurrPos()))
			{
				//If an enemy missile is inside a detonations boudning circle detonate 
				//it.
				++m_PlayerScore;
				Detonate(i->GetCurrPos());
				i = gEnemyMissiles.erase(i);
				CreateEnemyMissile();
			}
			else
			{
				++i;
			}
		}
		++j;
	}
}

//--------------------------------------------------------------------------
//Name: Detonate(Vec2 A) 
//Desc:	This method take a coordinate of a detonated missile, creates a new 
//		detonation instance and adds it to the detonation array.
//--------------------------------------------------------------------------
void GameApp::Detonate(Vec2 A)
{
	mDetonation = new Detonation(A);
	gMissileDet.push_back(*mDetonation);
}

//--------------------------------------------------------------------------
//Name: CreateEnemyMissile() 
//Desc:	Adds the last enemy missile to the list and creates a new instance.
//--------------------------------------------------------------------------
void GameApp::CreateEnemyMissile()
{
	
	gEnemyMissiles.push_back(*mEnemyMissile);
	mEnemyMissile = new EnemyMissile();
}

//--------------------------------------------------------------------------
//Name: DrawPlayerScore() 
//Desc:	Gets called by the Draw() method. We calculate the player scores in 
//		the ones, tens and hundreds positions and draw the corresponding 
//		sprite to represent the desired player score.
//--------------------------------------------------------------------------
void GameApp::DrawPlayerScore()
{
	mScore->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

	if (m_PlayerScore < 10)
	{
		mPlayerOnes[m_PlayerScore]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPlayerDecimals[0]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPlayerHundreds[0]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
	}
	else if (m_PlayerScore >= 10 && m_PlayerScore < 100)
	{
		mPlayerOnes[m_PlayerScore % 10]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPlayerDecimals[(int)(m_PlayerScore / 10)]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPlayerHundreds[0]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
	}
	else if (m_PlayerScore >= 100)
	{
		mPlayerOnes[m_PlayerScore % 10]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPlayerDecimals[(int)((m_PlayerScore % 100) / 10)]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
		mPlayerHundreds[(int)(m_PlayerScore / 100)]->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
	}
	else if (m_PlayerScore > 999)
	{
		m_PlayerScore = 0;
	}
	
}

//--------------------------------------------------------------------------
//Name: DrawPlayerLives() 
//Desc:	Called from the Draw() method. Uses the players lives data member 
//		and paints the heart sprite for each of the players lives. 
//--------------------------------------------------------------------------
void GameApp::DrawPlayerLives()
{
	if (m_PlayerLives == 3)
	{
		mLives->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

		mHeart->mPosition.x = 790;
		mHeart->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

		mHeart->mPosition.x = 810;
		mHeart->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

		mHeart->mPosition.x = 830;
		mHeart->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
	}

	else if (m_PlayerLives == 2)
	{
		mLives->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

		mHeart->mPosition.x = 790;
		mHeart->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

		mHeart->mPosition.x = 810;
		mHeart->draw(m_oBackBuffer->getDC(), m_hSpriteDC);
	}

	else if (m_PlayerLives == 1)
	{
		mLives->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

		mHeart->mPosition.x = 790;
		mHeart->draw(m_oBackBuffer->getDC(), m_hSpriteDC);

	}
	else if (m_PlayerLives <= 0)
	{
		m_GameOver = true;
		m_Paused = true;
	}
}

//--------------------------------------------------------------------------
//Name: LoadScoreSprites() 
//Desc:	Initializes the score text and numbers sprites and stores these 
//		înside the corresponding array.
//--------------------------------------------------------------------------
void GameApp::LoadScoreSprites(HINSTANCE hInstance)
{
	Circle	boundingCircle;
	Vec2	position0(0.0f, 0.0f);
	Vec2	velocity0(0.0f, 0.0f);

	position0.x = 55;
	position0.y = 10;

	mScore = new Sprite(hInstance, IDB_SCORE, IDB_SCOREMASK,
		boundingCircle, position0, velocity0);

	position0.x = 147;
	position0.y = 9;

	mPlayerOnes[0] = new Sprite (hInstance, IDB_ZERO, IDB_ZEROMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[1] = new Sprite(hInstance, IDB_ONE, IDB_ONEMASK,
		boundingCircle, position0, velocity0);
	
	mPlayerOnes[2] = new Sprite(hInstance, IDB_TWO, IDB_TWOMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[3] = new Sprite(hInstance, IDB_THREE, IDB_THREEMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[4] = new Sprite(hInstance, IDB_FOUR, IDB_FOURMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[5] = new Sprite(hInstance, IDB_FIVE, IDB_FIVEMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[6] = new Sprite(hInstance, IDB_SIX, IDB_SIXMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[7] = new Sprite(hInstance, IDB_SEVEN, IDB_SEVENMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[8] = new Sprite(hInstance, IDB_EIGHT, IDB_EIGHTMASK,
		boundingCircle, position0, velocity0);

	mPlayerOnes[9] = new Sprite(hInstance, IDB_NINE, IDB_NINEMASK,
		boundingCircle, position0, velocity0);


	position0.x = 131;
	position0.y = 9;

	mPlayerDecimals[0] = new Sprite(hInstance, IDB_ZERO, IDB_ZEROMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[1] = new Sprite(hInstance, IDB_ONE, IDB_ONEMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[2] = new Sprite(hInstance, IDB_TWO, IDB_TWOMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[3] = new Sprite(hInstance, IDB_THREE, IDB_THREEMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[4] = new Sprite(hInstance, IDB_FOUR, IDB_FOURMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[5] = new Sprite(hInstance, IDB_FIVE, IDB_FIVEMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[6] = new Sprite(hInstance, IDB_SIX, IDB_SIXMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[7] = new Sprite(hInstance, IDB_SEVEN, IDB_SEVENMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[8] = new Sprite(hInstance, IDB_EIGHT, IDB_EIGHTMASK,
		boundingCircle, position0, velocity0);

	mPlayerDecimals[9] = new Sprite(hInstance, IDB_NINE, IDB_NINEMASK,
		boundingCircle, position0, velocity0);

	position0.x = 115;
	position0.y = 9;

	mPlayerHundreds[0] = new Sprite(hInstance, IDB_ZERO, IDB_ZEROMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[1] = new Sprite(hInstance, IDB_ONE, IDB_ONEMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[2] = new Sprite(hInstance, IDB_TWO, IDB_TWOMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[3] = new Sprite(hInstance, IDB_THREE, IDB_THREEMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[4] = new Sprite(hInstance, IDB_FOUR, IDB_FOURMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[5] = new Sprite(hInstance, IDB_FIVE, IDB_FIVEMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[6] = new Sprite(hInstance, IDB_SIX, IDB_SIXMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[7] = new Sprite(hInstance, IDB_SEVEN, IDB_SEVENMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[8] = new Sprite(hInstance, IDB_EIGHT, IDB_EIGHTMASK,
		boundingCircle, position0, velocity0);

	mPlayerHundreds[9] = new Sprite(hInstance, IDB_NINE, IDB_NINEMASK,
		boundingCircle, position0, velocity0);

}