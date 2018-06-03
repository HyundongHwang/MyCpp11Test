#include "stdafx.h"

class MyItem {
public:
    MyItem(std::string id)
    {
        _id = id;
        std::cout << "MyItem.new : " << _id << std::endl;
    }

    virtual ~MyItem()
    {
        std::cout << "MyItem.~ : " << _id << std::endl;
    }

    std::string _id;
};

TEST(unordered_map_test, create_destroy)
{
    std::unordered_map<std::string, std::shared_ptr<MyItem>> umap;
    auto item0 = std::make_shared<MyItem>("0");
    auto item1 = std::make_shared<MyItem>("1");
    auto item2 = std::make_shared<MyItem>("2");
    umap["0"] = item0;
    umap["1"] = item1;
    umap["2"] = item2;
}