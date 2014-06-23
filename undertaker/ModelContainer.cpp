/*
 *   undertaker - analyze preprocessor blocks in code
 *
 * Copyright (C) 2009-2012 Reinhard Tartler <tartler@informatik.uni-erlangen.de>
 * Copyright (C) 2009-2011 Julio Sincero <Julio.Sincero@informatik.uni-erlangen.de>
 * Copyright (C) 2010-2011 Christian Dietrich <christian.dietrich@informatik.uni-erlangen.de>
 * Copyright (C) 2012 Ralf Hackner <rh@ralf-hackner.de>
 * Copyright (C) 2013-2014 Stefan Hengelein <stefan.hengelein@fau.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef DEBUG
#define BOOST_FILESYSTEM_NO_DEPRECATED
#endif

#include "ModelContainer.h"
#include "RsfConfigurationModel.h"
#include "CnfConfigurationModel.h"
#include "Logging.h"

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>


static const boost::regex model_regex("^([-[:alnum:]]+)\\.(model|cnf)$");

ConfigurationModel* ModelContainer::loadModels(std::string model) {
    ModelContainer *f = getInstance();
    int found_models = 0;
    std::list<std::string> filenames;
    ConfigurationModel *ret = nullptr;

    if (! boost::filesystem::exists(model)){
        logger << error << "model '" << model << "' doesn't exist (neither directory nor file)" << std::endl;
        return nullptr;
    }
    if (! boost::filesystem::is_directory(model)) {
        /* A model file was specified, so load exactly this one */
        boost::match_results<const char*> what;
        boost::filesystem::path p(model);
        std::string model_name = p.stem().string();

        ret = f->registerModelFile(model, model_name);
        if (ret) {
            logger << info << "loaded " << ret->getModelVersionIdentifier()
                   << " model for " << model_name << std::endl;
        } else {
            logger << error << "failed to load model from " << model
                   << std::endl;
        }
        return ret;
    }

    for (boost::filesystem::directory_iterator dir(model);
         dir != boost::filesystem::directory_iterator();
         ++dir) {
#if !defined(BOOST_FILESYSTEM_VERSION) || BOOST_FILESYSTEM_VERSION == 2
        filenames.push_back(dir->path().filename());
#else
        filenames.push_back(dir->path().filename().string());
#endif
    }
    filenames.sort();

    for (std::string &filename : filenames) {
        boost::match_results<const char*> what;

        if (boost::regex_search(filename.c_str(), what, model_regex)) {
            std::string found_arch = what[1];
            ModelContainer::iterator a = f->find(found_arch);

            if (a == f->end()) { // not found
                ConfigurationModel *mod = f->registerModelFile(model + "/" + filename.c_str(), found_arch);
                /* overwrite the return value */
                if (mod) ret = mod;
                found_models++;

                logger << info << "loaded " << mod->getModelVersionIdentifier()
                       << " model for " << found_arch << std::endl;
            }
        }
    }
    if (found_models > 0) {
        logger << info << "found " << found_models << " models" << std::endl;
        return ret;
    } else {
        logger << error << "could not find any models" << std::endl;
        return nullptr;
    }
}

// parameter filename will look like: 'models/x86.model', string like 'x86'
ConfigurationModel *ModelContainer::registerModelFile(std::string filename, std::string arch) {
    ConfigurationModel *db;
    /* Was already loaded */
    if ((db = lookupModel(arch.c_str()))) {
        logger << info << "A model for " << arch << " was already loaded" << std::endl;
        return db;
    }
    boost::filesystem::path filepath(filename);
    if (filepath.extension() == ".cnf") {
        db = new CnfConfigurationModel(filename.c_str());
    } else {
        db = new RsfConfigurationModel(filename.c_str());
    }
    if (!db) {
        logger << error << "Failed to load model from " << filename
               << std::endl;
    }
    this->emplace(arch, db);
    return db;
};

ConfigurationModel *ModelContainer::lookupModel(const char *arch)  {
    ModelContainer *f = getInstance();
    // first step: look if we have it in our models list;
    ModelContainer::iterator a = f->find(arch);
    if (a != f->end()) {
        // we've found it in our map, so return it
        return a->second;
    } else {
        // No model was found
        return nullptr;
    }
}

const char *ModelContainer::lookupArch(const ConfigurationModel *model) {
    for (auto &entry : *getInstance())  // pair<string, ConfigurationModel *>
        if (entry.second == model)
            return entry.first.c_str();

    return nullptr;
}

ConfigurationModel *ModelContainer::lookupMainModel() {
    ModelContainer *f = getInstance();
    return ModelContainer::lookupModel(f->main_model.c_str());
}

void ModelContainer::setMainModel(std::string main_model) {
    ModelContainer *f = getInstance();

    if (!ModelContainer::lookupModel(main_model.c_str())) {
        logger << error << "Could not specify main model "
               << main_model << ", because no such model is loaded" << std::endl;
        return;
    }
    logger << info << "Using " << main_model << " as primary model" << std::endl;
    f->main_model = main_model;
}

const char *ModelContainer::getMainModel() {
    ModelContainer *f = getInstance();
    return f->main_model.c_str();
}


ModelContainer *ModelContainer::getInstance() {
    static std::unique_ptr<ModelContainer> instance;
    if (!instance) {
        instance = std::unique_ptr<ModelContainer>(new ModelContainer());
    }
    return instance.get();
}

ModelContainer::~ModelContainer() {
    for (auto &entry : *this)  // pair<string, ConfigurationModel *>
        delete entry.second;
}
