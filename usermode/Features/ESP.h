#pragma once
#include "..\Core\Render.h"
#include "..\Core\Config.h"

namespace ESP
{
	void RenderPlayerESP(const CEntity& LocalEntity, const CEntity& Entity, ImVec4 Rect, int LocalPlayerControllerIndex, int Index)
	{
		if (Entity.Pawn.Pos == Vec3(0, 0, 0))
			return;

		Render::DrawBone(Entity, ESPConfig::BoneColor, 1.3f);
		Render::DrawHeadCircle(Entity, ESPConfig::HeadBoxColor);
	}

	void RenderPreview(ImVec2 windowSize)
	{
		if (!ESPConfig::ShowPreview)
			return;

		auto drawList = ImGui::GetWindowDrawList();
		auto& io = ImGui::GetIO();

		const ImVec2 rectSize(100, 150);
		const ImVec2 rectPos((windowSize.x - rectSize.x) * 0.45f, (windowSize.y - rectSize.y) * 0.3f);
		ImVec2 centerPos = ImGui::GetCursorScreenPos();
		centerPos.x += rectPos.x;
		centerPos.y += rectPos.y * -1.20f;

		if (ESPConfig::ShowBoneESP) {
			const ImU32 boneColor = ESPConfig::BoneColor;
			struct BoneSegment { ImVec2 startOffset, endOffset; };
			const BoneSegment segments[] = {
				{ {50, 25}, {60, 55} },
				{ {60, 55}, {62, 65} },
				{ {62, 65}, {65, 70} },
				{ {65, 70}, {60, 100} },
				{ {60, 100}, {68, 145} },
				{ {62, 65}, {35, 100} },
				{ {35, 100}, {47, 130} },
				{ {50, 25}, {60, 30} },
				{ {60, 30}, {45, 55} },
				{ {45, 55}, {25, 45} },
				{ {50, 25}, {40, 30} },
				{ {40, 30}, {27, 53} },
				{ {27, 53}, {20, 45} }
			};
			for (const auto& seg : segments) {
				ImVec2 start(centerPos.x + seg.startOffset.x, centerPos.y + seg.startOffset.y);
				ImVec2 end(centerPos.x + seg.endOffset.x, centerPos.y + seg.endOffset.y);
				drawList->AddLine(start, end, boneColor, 1.8f);
			}
		}

		if (ESPConfig::ShowHeadBox) {
			drawList->AddCircle({ centerPos.x + 44, centerPos.y + 17 }, 12.0f, ESPConfig::HeadBoxColor, 0, 1.8f);
		}
	}

	void RenderOutOfFOVArrow(const CEntity& LocalEntity, const CEntity& Entity) {}
}
