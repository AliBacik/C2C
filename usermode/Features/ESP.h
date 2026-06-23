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

	void RenderOutOfFOVArrow(const CEntity& LocalEntity, const CEntity& Entity)
	{
		if (!ESPConfig::ShowOutOfFOVArrow) return;
		if (!Entity.IsAlive()) return;
		if (Entity.IsInScreen()) return;

		auto* drawList = ImGui::GetBackgroundDrawList();
		const ImVec2& io = ImGui::GetIO().DisplaySize;
		ImVec2 center(io.x * 0.5f, io.y * 0.5f);
		float radius = (std::min)(io.x, io.y) * ESPConfig::OutOfFOVRadiusFactor;

		// local player yaw'a gore dusman acisi
		float localYaw = LocalEntity.Pawn.ViewAngle.y * (3.14159265f / 180.f);
		Vec3 delta = Entity.Pawn.Pos - LocalEntity.Pawn.Pos;
		float angle = atan2f(delta.y, delta.x) - localYaw - (3.14159265f / 2.f);

		ImVec2 arrowPos(center.x + cosf(angle) * radius, center.y + sinf(angle) * radius);

		// ok ucgeni
		const float arrowSize = 8.f;
		ImVec2 dir(cosf(angle), sinf(angle));
		ImVec2 perp(-dir.y * arrowSize * 0.5f, dir.x * arrowSize * 0.5f);
		ImVec2 tip(arrowPos.x + dir.x * arrowSize, arrowPos.y + dir.y * arrowSize);
		ImVec2 base1(arrowPos.x - dir.x * arrowSize + perp.x, arrowPos.y - dir.y * arrowSize + perp.y);
		ImVec2 base2(arrowPos.x - dir.x * arrowSize - perp.x, arrowPos.y - dir.y * arrowSize - perp.y);

		drawList->AddTriangleFilled(tip, base1, base2, ESPConfig::OutOfFOVArrowColor);
	}

	inline ImVec4 GetBoxRect(const CEntity& Entity, int boxType)
	{
		if (boxType == 1)
			return Render::Get2DBoneRect(Entity);
		return Render::Get2DBox(Entity);
	}
}
