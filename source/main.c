#include <citro2d.h>

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
	C2D_Sprite spr;
	float dx, dy; // velocity
} Sprite;

static C2D_SpriteSheet spriteSheet;
static Sprite sprites[2];

//---------------------------------------------------------------------------------
static void initSprites() {
//---------------------------------------------------------------------------------
  // Top Screen
  Sprite* top_screen_sprite = &sprites[0];
  C2D_SpriteFromSheet(&top_screen_sprite->spr, spriteSheet, 0);
  C2D_SpriteSetPos(&top_screen_sprite->spr, 0, 0);

  // Bottom Screen
  Sprite* bottom_screen_sprite = &sprites[1];
  C2D_SpriteFromSheet(&bottom_screen_sprite->spr, spriteSheet, 1);
  C2D_SpriteSetPos(&bottom_screen_sprite->spr, 0, 0);
}

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	// consoleInit(GFX_BOTTOM, NULL);

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
  C3D_RenderTarget* bottom = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_RIGHT);

	// Load graphics
	spriteSheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
	if (!spriteSheet) svcBreak(USERBREAK_PANIC);

	// Initialize sprites
	initSprites();

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu

		// printf("\x1b[2;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		// printf("\x1b[3;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		// printf("\x1b[4;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);
    C2D_DrawSprite(&sprites[0].spr);
    C2D_TargetClear(bottom, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(bottom);
    C2D_DrawSprite(&sprites[1].spr);
		C3D_FrameEnd(0);
	}

	// Delete graphics
	C2D_SpriteSheetFree(spriteSheet);

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
