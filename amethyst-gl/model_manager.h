#pragma once
/***********************************************************************
 Amethyst-GL
 - Model manager and factory

 Authors (c):
 2020 Beau V.C. Bellamy (bellamy.beau@gmail.com)
 ***********************************************************************/

#include "model.h"

#include <string>
#include <unordered_map>

namespace amethyst::client {

class ModelManager {

  public:
    ModelManager();
   ~ModelManager();

    void add(Model::sptr newmodel);
    Model::sptr get(std::string &model_name);
    Model::sptr find(const std::string &name);
    void clear(void);

  private:
    ModelManager(const ModelManager&) = delete;
    ModelManager& operator=(const ModelManager&) = delete;

    std::unordered_map<std::string, Model::sptr>  models_;
};

extern ModelManager modelmanager;


} // namsspace amethyst::client

