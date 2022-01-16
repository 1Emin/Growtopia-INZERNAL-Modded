#include <menu/menu.h>
#include <sdk/sdk.h>
#include <sdk/Consts.h>
#include <sdk/ResourceManager.h>
#include <hooks/SendPacket.h>
#include <hooks/SendPacketRaw.h>
#include "sdk/world/WorldCamera.h"
#include "core/globals.h"
#include "proton/Math/rtRect.h"
#include <hooks/ProcessTankUpdatePacket.h>
#include <algorithm>

void menu::other() {





    //msgtoall

    auto logic = sdk::GetGameLogic();
    auto local = sdk::GetGameLogic()->GetLocalPlayer();
    auto locals = sdk::GetGameLogic()->GetNetObjMgr();
    auto tilemap = logic->GetTileMap();
    auto w2s = logic->GetWorldRenderer()->camera;

    auto draw = ImGui::GetBackgroundDrawList();
    if (local) {
        auto pos2f = local->GetPos();
        float tilex = pos2f.x / 32;
        float tiley = pos2f.y / 32;
        ImGui::Text("Position: %.0f,%.0f", pos2f.x, pos2f.y);
        ImGui::Text("Pos Tile: %.0f,%.0f", tilex, tiley);
        if (local->flags != 0) {
            printf("Flags %d \n", local->flags);
        }
        ImGui::Text("Facing Left: %d", local->facing_left);
        string test = std::to_string(local->skin_color);
        imwrap::inputstring("##test", &test, ImGuiInputTextFlags_None);
        Tile* tile = tilemap->GetTileSafe(local->GetPos() / 32);
        if (tile) {
            ImGui::Text("%d", tile->foreground);
            ImGui::Text("%d", tile->flags);
            ImGui::Text("%d", tile->collision_rect);
        }
    }
    ImGui::Checkbox("ItemFilter", &opt::cheat::filterautocollect);
    ImGui::SameLine();
    ImGui::InputInt("##ITEMID", &opt::cheat::itemfilter);
    ImGui::Checkbox("Auto Collect", &opt::cheat::autocollect);
    ImGui::SameLine();
    ImGui::SliderInt("Range", &opt::cheat::range_val, 1, 10);
    ImGui::Checkbox("Auto Farm", &opt::cheat::autofarm);
    ImGui::SameLine();
    ImGui::InputInt("Item ID", &opt::cheat::itemid_val);


    //test
    bool button_state = false;
    struct Item
    {
        int itemID;
        std::string itemName;

        Item(int itemID, std::string itemName)
        {
            this->itemID = itemID;
            this->itemName = itemName;
        }
    };

    Item* items[11]; // total item
    items[0] = new Item(1, "1"); 
    items[1] = new Item(2, "2");
    items[2] = new Item(3, "3");
    items[3] = new Item(4, "4");
    items[4] = new Item(5, "5");
    items[5] = new Item(6, "6");
    items[6] = new Item(7, "7");
    items[7] = new Item(8, "8");
    items[8] = new Item(9, "9");
    items[9] = new Item(10, "10");
    items[10] = new Item(11, "11");
    items[11] = new Item(12, "12");

        int ID = 0;

    std::string idstr = "";
    static char buf1[16] = ""; ImGui::InputText("<- Item Name", buf1, 64);
    std::string itemName = "";


    int size = sizeof(buf1) / sizeof(char);
    for (int i = 0; i < size; i++) {

        itemName = itemName + buf1[i];
    }
    transform(itemName.begin(), itemName.end(), itemName.begin(), std::tolower);
    for (int i = 0; i < 11; i++) { // 1 item sayısı
        if (itemName.find(items[i]->itemName) != -1) {
            ID = items[i]->itemID;
        }
    }


    ImGui::Text(("Item ID : " + std::to_string(ID)).c_str());



    //msg
    if (ImGui::BeginChild("###Warp", AUTOSIZEC(2), true, ImGuiWindowFlags_MenuBar)) {
        ImGui::BeginMenuBar();
        ImGui::Text("Msg to All");
        ImGui::EndMenuBar();
        static char worlddname[72];
        size_t size = std::strlen(worlddname);
        ImGui::Text("Message:");
        ImGui::SameLine();
        ImGui::InputText("", worlddname, IM_ARRAYSIZE(worlddname));
        ImGui::SameLine();
        if (ImGui::Button("Send MSG")) {

            for (auto pair : sdk::GetGameLogic()->GetNetObjMgr()->players) {
                std::string name = pair.second->name;
                std::string name2 = name.substr(2);
                utils::replace(name2, "``", "");
                std::string packet = "action|input\n|text|/msg " + name2 + +"  " + "`#[Sahyui] `w: " + worlddname;
                SendPacketHook::Execute(2, packet, sdk::GetPeer());


            }
            ImGui::SameLine();
            ImGui::Text(" %u / 72", size);
            ImGui::EndChild();
            ImGui::Spacing();
        }
    }




















}
