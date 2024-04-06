#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class olcPanZoom : public olc::PixelGameEngine
{
public:
	olcPanZoom()
	{
		sAppName = "Panning and zooming";
	}

public:

	olc::vf2d PixelToWorld(olc::vi2d PixelPos)
	{
		return (olc::vf2d(PixelPos) / (float(ScreenWidth()) * scale) + offset) * olc::vf2d(1.0f, -1.0f);
	}

	olc::vi2d WorldToPixel(olc::vf2d WorldPos)
	{
		return olc::vi2d((float)ScreenWidth() * (WorldPos * olc::vf2d(1.0f,-1.0f) - offset) * scale);
	}

	void drawCornerWorldPos()
	{
		DrawString(1, ScreenHeight()-9, std::to_string(PixelToWorld({0,ScreenHeight() }).x) + ", " + std::to_string(PixelToWorld({0,ScreenHeight() }).y));
		DrawString(ScreenWidth() / 2, ScreenHeight()-9, std::to_string(PixelToWorld({ ScreenWidth(),ScreenHeight() }).x) + ", " + std::to_string(PixelToWorld({ ScreenWidth(),ScreenHeight() }).y));
		DrawString(1, 1, std::to_string(PixelToWorld({ 0,0 }).x) + ", " + std::to_string(PixelToWorld({ 0,0 }).y));
		DrawString(ScreenWidth()/2, 1, std::to_string(PixelToWorld({ ScreenWidth(),0}).x) + ", " + std::to_string(PixelToWorld({ ScreenWidth(),0}).y));
	}

	float scale = 1.0f;
	olc::vf2d offset;

	olc::vf2d ZoomWorldMousePos;
	olc::vf2d PanPixelMousePos;
	olc::vf2d PanInitialOffset;

	float aspectRatio;

	bool OnUserCreate() override
	{
		offset = { -0.5f, -float(ScreenHeight()) / float(2 * ScreenWidth()) };

		aspectRatio = float(ScreenHeight()) / float(ScreenWidth());

		std::cout << PixelToWorld({ 0,ScreenHeight() }).y << "\n";

		drawCornerWorldPos();
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		if(GetMouseWheel() != 0)
			std::cout << GetMouseWheel() << "\n";

		if (GetKey(olc::Key::CTRL).bHeld)
		{
			if (GetMouse(0).bHeld)
			{
				if (GetMouse(0).bPressed)
				{
					PanPixelMousePos = GetMousePos();
					PanInitialOffset = offset;
				}

				offset = PanInitialOffset + olc::vf2d(PanPixelMousePos - GetMousePos()) / (scale * float(ScreenWidth()));
			}

			if (GetMouse(1).bHeld)
			{
				if (GetMouse(1).bPressed)
					ZoomWorldMousePos = olc::vf2d(GetMousePos()) / float(ScreenWidth()) / scale + offset;

				if (GetKey(olc::Key::SHIFT).bHeld)
				{
					scale *= 0.999f;
					offset = offset - (ZoomWorldMousePos - offset) * 0.001f;
				}
				else
				{
					scale *= 1.001f;
					offset = offset + (ZoomWorldMousePos - offset) * 0.001f;
				}
			}
			
		}
		
		Clear(olc::BLACK);

		DrawLine(WorldToPixel({ -0.5,0.0f }), WorldToPixel({ 0.5f, 0.0f }));
		DrawLine(WorldToPixel({ 0.0f,-aspectRatio/2.0f }), WorldToPixel({ 0.0f, aspectRatio/2.0f }));
		DrawTriangle(WorldToPixel({ 0.0f, 0.0f }), WorldToPixel({ 0.5f, 0.1f }), WorldToPixel({ 0.3f, 0.4f }));
		FillCircle(WorldToPixel({ -0.2f, 0.25f }), 5 * scale);

		drawCornerWorldPos();

		return true;
	}
};

int main()
{
	olcPanZoom demo;
	if (demo.Construct(500, 250, 1,1))
		demo.Start();
	return 0;
}