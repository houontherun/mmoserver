---------------------------------------------------
-- auth： wupeifeng
-- date： 2017/2/6
-- desc： aoe（自身）+buff
-- 对前方【4】米范围内，最多【1】个目标造成【2】的伤害。并有【3】的概率给目标附加【buff 1】和【buff 2】                                                                   
---------------------------------------------------

require "Common/basic/functions"
require "Common/combat/Skill/SkillManager"
require "Common/combat/Skill/SkillAPI"


local skill_type = '6'

local Skill = require "Common/combat/Skill/Skill"
local SkillLocal = ExtendClass(Skill)

function SkillLocal:__ctor(scene, skill_id, data)
    local config = GetConfig("growing_skill")
    self.skill_data = config.Skill[tonumber(skill_id)]

    -- 目标类型
    self.cast_target_type = CastTargetType.CAST_TARGET_NONE
end

function SkillLocal:OnCastChannel(data)
    Skill.OnCastChannel(self, data)

    local function MakeBuff(target)
        if self.skill_data.Buff1 ~= 0 then
            target.skillManager:AddBuffFromSkill(tostring(self.skill_data.Buff1), self)
        end
        if self.skill_data.Buff2 ~= 0 then
            target.skillManager:AddBuffFromSkill(tostring(self.skill_data.Buff2), self)
        end
    end

    local units = SkillAPI.EnumUnitsRandomInCircle(
        self.owner, 
        MathHelper.GetForward(self.owner, self:GetPara(4)),
        self:GetRangeParams(), 
        nil, self:GetPara(1), true)

    for _, unit in pairs(units) do
        SkillAPI.TakeDamage(unit, self.owner, self, self.OnDamageCorrect, self)
        SkillAPI.RandEvent(self:GetPara(3)/10000, MakeBuff, unit)
    end

   
end

function SkillLocal:OnDamageCorrect(M1, M2, M3, M4, target)
    local percent = self:GetPara(2) / 10000
    return M4 * percent
end

SkillLib[skill_type] = SkillLocal 

return SkillLocal