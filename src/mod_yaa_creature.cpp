#include "CreatureScript.h"
#include "Player.h"
#include "ScriptedGossip.h"
#include "Chat.h"

class YaaCreature : public CreatureScript
{
public:
  YaaCreature() : CreatureScript("npc_yaa") {}

  bool OnGossipHello(Player *player, Creature *creature) override
  {
    ClearGossipMenuFor(player);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Reset talents", GOSSIP_SENDER_MAIN, 1);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Reset instances", GOSSIP_SENDER_MAIN, 2);
    AddGossipItemFor(player, GOSSIP_ICON_CHAT, "Level professions to WotlK", GOSSIP_SENDER_MAIN, 3);
    SendGossipMenuFor(player, 6000100, creature->GetGUID());
    return true;
  }

  bool OnGossipSelect(Player *player, Creature *creature, uint32 sender, uint32 action) override
  {
    if (sender != GOSSIP_SENDER_MAIN)
    {
      return false;
    }

    if (action == 1)
    {
      ClearGossipMenuFor(player);
      player->resetTalents(true);
      player->SendTalentsInfoData(false);
      player->CastSpell(player, 31726);
    }
    else if (action == 2)
    {
      ClearGossipMenuFor(player);
      for (uint8 i = 0; i < MAX_DIFFICULTY; ++i)
      {
        BoundInstancesMap const &m_boundInstances = sInstanceSaveMgr->PlayerGetBoundInstances(player->GetGUID(), Difficulty(i));
        for (BoundInstancesMap::const_iterator itr = m_boundInstances.begin(); itr != m_boundInstances.end();)
        {
          if (itr->first != player->GetMapId())
          {
            sInstanceSaveMgr->PlayerUnbindInstance(player->GetGUID(), itr->first, Difficulty(i), true, player);
            itr = m_boundInstances.begin();
          }
          else
            ++itr;
        }
      }
      player->CastSpell(player, 59908);
    }
    else if (action == 3)
    {
      CloseGossipMenuFor(player);
      uint16 SkillIds[14] = {129, 164, 165, 171, 182, 185, 186, 197, 202, 333, 356, 393, 755, 773};
      for(int i = 0; i < 14; i++)
      {
        if(player->HasSkill(SkillIds[i]) && player->GetSkillValue(SkillIds[i]) < 350)
        {
          player->SetSkill(SkillIds[i], player->GetSkillStep(SkillIds[i]), 350, 450);
        }
      }
    }

    return true;
  }
};

void AddSC_yaa_creature()
{
  new YaaCreature();
}