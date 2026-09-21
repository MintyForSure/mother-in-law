//
// Created by tailofhell on 9/16/26.
//

#include "items.h"

#include <string>
#include <utility>

class item {
    public:
    std::string itemName;
    std::string itemPrettyName;
    std::string itemDescription;
    bool consumable{};
    bool keyItem{};
    std::string type;
    std::string occasion; //battle, outside battle, always, never
    bool curesPoison{};
    bool curesParalyzed{};
    bool curesCrying{};
    bool curesFeelingStrange{};
    //
    int restoresHP{};
    int restoresPP{};
    std::string scope; //one member, all members
    void curesWhat(bool poison,bool paralyzed,bool crying,bool feelingStrange) {
        curesPoison=poison;
        curesParalyzed=paralyzed;
        curesCrying=crying;
        curesFeelingStrange=feelingStrange;
    }
    void setItemName(std::string name,std::string desc, std::string prettyName) {
        itemName = std::move(name);
        itemDescription = std::move(desc);
        itemPrettyName = std::move(prettyName);
    }
};

class healingItem : public item {
    private:
    bool curesPoison{};
    bool curesParalyzed{};
    bool curesCrying{};
    bool curesFeelingStrange{};
    public:
    int restoresHP{};
    int restoresPP{};
    std::string scope; //one member, all members
    void curesWhat(bool poison,bool paralyzed,bool crying,bool feelingStrange) {
        curesPoison=poison;
        curesParalyzed=paralyzed;
        curesCrying=crying;
        curesFeelingStrange=feelingStrange;
    }
};

int itemData(const std::string &name) {
    int temp=0;
    if (name == "coolFood") {
        item coolFood;
        coolFood.setItemName("coolFood","Food that's really cool.","Cool Food");
        coolFood.consumable=true;
        coolFood.keyItem=false;
        coolFood.occasion="always";
        coolFood.type="healing";
        coolFood.restoresHP=30;
        coolFood.restoresPP=5;
        coolFood.scope="one";
        coolFood.curesWhat(false,false,false,false);
        return temp;
    }

    item strangeStick;
    strangeStick.setItemName("strangeStick","An odd stick you can't do anything with.","Strange Stick");
    strangeStick.consumable=true;
    strangeStick.keyItem=false;
    strangeStick.type="ordinary";
    strangeStick.occasion="never";
    return temp;
}
