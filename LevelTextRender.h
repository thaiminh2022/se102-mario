#pragma once
#include "GameObject.h"
class LevelTextRender :
    public GameObject
{
public:
	Rect GetBoundingBox() override;
	void Render() override;
	bool IsActive() override { return true; }
	LevelTextRender(const vector<WorldTextData>& texts);
private:
	vector<WorldTextData> texts;
};
