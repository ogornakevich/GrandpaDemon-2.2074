#ifndef LISTMANAGER_H
#define LISTMANAGER_H

#include <vector>
#include <string>
#include <sstream>

#include <Geode/Geode.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

class ListManager {
public:
    inline static std::vector<int> demonIDList;
    inline static bool firstTimeOpen = true;
    inline static int filterType = 0;
    inline static bool isSupremeSearching = false;

    inline static void parseRequestString(std::string const& str) {
        demonIDList.clear();

        auto parsed = matjson::parse(str);
        if (!parsed) {
            log::error("Failed to parse AREDL JSON");
            return;
        }

        auto json = parsed.unwrap();

        
        if (!json.isArray()) {
            log::error("AREDL JSON is not an array");
            return;
        }

        
        for (auto& level : json.asArray().unwrap()) {
            
            if (!level.isObject()) continue;

            if (!level.contains("level_id")) continue;
            
            auto levelId = level["level_id"];
            
            if (!levelId.isNumber()) continue;

            
            demonIDList.push_back(levelId.asInt().unwrap());
        }

        log::info("Loaded {} AREDL demon IDs", demonIDList.size());
    }


    inline static int getPositionOfID(int id) {
        for (size_t i = 0; i < demonIDList.size(); i++) {
            if (demonIDList[i] == id)
                return static_cast<int>(i);
        }
        return -1;
    }

    inline static CCSprite* getSpriteFromPosition(int pos, bool hasText) {
        if (pos <= 499 && pos > 249)
            return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon0_text.png"_spr : "GrD_demon0.png"_spr);

        if (pos <= 249 && pos > 149)
            return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon1_text.png"_spr : "GrD_demon1.png"_spr);

        if (pos <= 149 && pos > 74)
            return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon2_text.png"_spr : "GrD_demon2.png"_spr);

        if (pos <= 74 && pos > 24)
            return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon3_text.png"_spr : "GrD_demon3.png"_spr);

        if (pos <= 24 && pos > 0)
            return CCSprite::createWithSpriteFrameName(hasText ? "GrD_demon4_text.png"_spr : "GrD_demon4.png"_spr);

        if (pos == 0) {
            bool disabled = Mod::get()->getSettingValue<bool>("grandpa-demon-disable");
            return CCSprite::createWithSpriteFrameName(
                hasText
                    ? (disabled ? "GrD_demon4_text.png"_spr : "GrD_demon5_text.png"_spr)
                    : (disabled ? "GrD_demon4.png"_spr : "GrD_demon5.png"_spr)
            );
        }

        return nullptr;
    }

    inline static GJSearchObject* getSearchObject(int upper, int lower) {
        std::stringstream download;
        bool first = true;

        if (!(upper == 0 && lower == 0)) {
            for (int i = upper; i > lower; i--) {
                if (!first)
                    download << ",";
                download << demonIDList.at(i);
                first = false;
            }
        } else if (!demonIDList.empty()) {
            download << demonIDList.at(0);
        }

        download << "&gameVersion=22";
        return GJSearchObject::create(SearchType::Type19, download.str());
    }
};

#endif