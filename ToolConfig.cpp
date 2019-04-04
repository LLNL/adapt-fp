/*
 * Copyright (c) 2019, Lawrence Livermore National Security, LLC.
 * Produced at the Lawrence Livermore National Laboratory.
 *
 * Written by:  Logan Moody, moody25@llnl.gov
 *
 * LLNL-CODE-762758. All rights reserved.
 *
 * This file is part of ADAPT. For details, see https://github.com/LLNL/adapt-fp
 *
 * Please also see the LICENSE file for our notice and the GPL.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the terms and conditions of the GNU General
 * Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ToolConfig.hpp"
#include <unistd.h>

#define MAXPATHLEN 255

//////////////
//ToolAction//
//////////////
ToolAction::ToolAction(std::string actionType) {
    action = actionType;
    name = "";
    handle = "";
    scope = "";
    sourceInfo = "";
    fromType = "";
    toType = "";
    error = -1;
    assignments = -1;
}

ToolAction::ToolAction() {
    action = "None";
    name = "";
    handle = "";
    scope = "";
    sourceInfo = "";
    fromType = "";
    toType = "";
    error = -1;
    assignments = -1;
}

//actionType
std::string ToolAction::getActionType() {
    return action;
}

void ToolAction::setActionType(std::string type) {
    action = type;
}

//name
std::string ToolAction::getName() {
    return name;
}

void ToolAction::setName(std::string name) {
    this->name = name;
}

//handle
std::string ToolAction::getHandle() {
    return handle;
}

void ToolAction::setHandle(std::string handle) {
    this->handle = handle;
}

//scope
std::string ToolAction::getScope() {
    return scope;
}

void ToolAction::setScope(std::string scope) {
    this->scope = scope;
}

//sourceInfo
std::string ToolAction::getSourceInfo() {
    return sourceInfo;
}

void ToolAction::setSourceInfo(std::string source) {
    sourceInfo = source;
}

//fromType
std::string ToolAction::getFromType() {
    return fromType;
}

void ToolAction::setFromType(std::string type) {
    fromType = type;
}

//toType
std::string ToolAction::getToType() {
    return toType;
}

void ToolAction::setToType(std::string type) {
    toType = type;
}

//error
double ToolAction::getError() {
    return error;
}

void ToolAction::setError(double error) {
    this->error = error;
}

//assignments
long ToolAction::getAssignments() {
    return assignments;
}

void ToolAction::setAssignments(long assignments) {
    this->assignments = assignments;
}

void to_json(json& j, const ToolAction& a) {
    ToolAction temp = a;

    if (temp.getActionType() == "None") {
        return;
    }
    j["action"] = temp.getActionType();

    if (temp.getName() != "") {
        j["name"] = temp.getName();
    }

    if (temp.getHandle() != "") {
        char buff[MAXPATHLEN];
        std::string pwd = getcwd(buff, MAXPATHLEN);
        std::string handle = temp.getHandle();
        size_t loc = handle.find(pwd);
        if (loc != std::string::npos) {
            handle.replace(loc, pwd.length(), "${PWD}");
        }
        j["handle"] = handle;
    }

    if (temp.getScope() != "") {
        j["scope"] = temp.getScope();
    }

    if (temp.getSourceInfo() != "") {
        j["source_info"] = temp.getSourceInfo();
    }

    if (temp.getFromType() != "") {
        j["from_type"] = temp.getFromType();
    }

    if (temp.getToType() != "") {
        j["to_type"] = temp.getToType();
    }

    if (temp.getError() != -1) {
        j["error"] = temp.getError();
    }

    if (temp.getAssignments() != -1) {
        j["dynamic_assignments"] = temp.getAssignments();
    }
}

void from_json(const json& j, ToolAction& a) {
    try {
        a.setActionType(j.at("action"));
    } catch(...) {
        a.setActionType("None");
    }

    try {
        a.setName(j.at("name"));
    } catch(...) {
        a.setName("");
    }

    try {
        char buff[MAXPATHLEN];
        std::string pwd = getcwd(buff, MAXPATHLEN);
        std::string handle = j.at("handle");
        size_t loc = handle.find("${PWD}");
        if (loc != std::string::npos) {
            handle.replace(loc, 6, pwd);
        }
        a.setHandle(handle);
    } catch(...) {
        a.setHandle("");
    }

    try {
        a.setScope(j.at("scope"));
    } catch(...) {
        a.setScope("");
    }

    try {
        a.setSourceInfo(j.at("source_info"));
    } catch(...) {
        a.setSourceInfo("");
    }

    try {
        a.setFromType(j.at("from_type"));
    } catch(...) {
        a.setFromType("");
    }

    try {
        a.setToType(j.at("to_type"));
    } catch(...) {
        a.setToType("");
    }

    try {
        a.setError(j.at("error"));
    } catch(...) {
        a.setError(-1);
    }

    try {
        a.setAssignments(j.at("dynamic_assignments"));
    } catch(...) {
        a.setAssignments(-1);
    }
}


//////////////
//ToolConfig//
//////////////
ToolConfig::ToolConfig() {
    this->version = newVersion;
    this->sourceFiles = {};
    this->executable = "";
    this->toolID = "None";
    this->actions = {};
}

ToolConfig::ToolConfig(std::string fileName) {
    std::ifstream in(fileName);
    json config;
    in >> config;
    from_json(config, *this);
}

//sourceFiles
std::vector<std::string>& ToolConfig::getSourceFiles() {
    return sourceFiles;
}

//executable
std::string ToolConfig::getExecutable() {
    return executable;
}

void ToolConfig::setExecutable(std::string exe) {
    executable = exe;
}

//toolID
std::string ToolConfig::getToolID() {
    return toolID;
}

void ToolConfig::setToolID(std::string toolID) {
    this->toolID = toolID;
}

//version
std::string ToolConfig::getVersion() {
    return this->version;
}

void ToolConfig::setVersion(std::string v) {
    this->version = v;
}

//actions
void ToolConfig::addAction(ToolAction action) {
    actions.push_back(action);
}

//TF
void ToolConfig::addReplaceVarType(std::string handle, std::string var_name, std::string scope, std::string source, std::string fromType, std::string toType) {
    ToolAction action("change_var_type");
    action.setHandle(handle);
    action.setName(var_name);
    action.setScope(scope);
    action.setSourceInfo(source);
    action.setFromType(fromType);
    action.setToType(toType);
    addAction(action);
}

void ToolConfig::addReplaceVarBaseType(std::string handle, std::string var_name, std::string scope, std::string source, std::string fromType, std::string toType) {
    ToolAction action("change_var_basetype");
    action.setHandle(handle);
    action.setName(var_name);
    action.setScope(scope);
    action.setSourceInfo(source);
    action.setFromType(fromType);
    action.setToType(toType);
    addAction(action);
}

//AdaPT
void ToolConfig::addReplaceVarType(std::string handle, std::string var_name, double error, long assignments) {
    ToolAction action("change_var_type");
    action.setError(error);
    action.setAssignments(assignments);
    action.setHandle(handle);
    action.setName(var_name);
    addAction(action);
}

void ToolConfig::addReplaceVarBaseType(std::string handle, std::string var_name, double error, long assignments) {
    ToolAction action("change_var_basetype");
    action.setError(error);
    action.setAssignments(assignments);
    action.setHandle(handle);
    action.setName(var_name);
    addAction(action);
}

std::vector<ToolAction>& ToolConfig::getActions() {
    return actions;
}

bool ToolConfig::saveConfig(std::string fileName) {
    std::ofstream out(fileName);
    if (!out.is_open()) {
        return false;
    }
    json config;
    to_json(config, *this);
    //ugly output
    //out << config;
    //pretty output
    out << std::setw(4) << config << std::endl;
    return true;
}

void to_json(json &j, const ToolConfig &a) {
    ToolConfig temp = a;

    std::vector<std::string>& files = temp.getSourceFiles();
    if(files.size() > 0) {
        j["source_files"] = files;
    }

    if (temp.getExecutable() != "") {
        j["executable"] = temp.getExecutable();
    }

    j["tool_id"] = temp.getToolID();
    j["version"] = temp.getVersion();

    std::vector<ToolAction>& actions = temp.getActions();
    if(actions.size() > 0) {
        j["actions"] = actions;
    }
}

void from_json(const json& j, ToolConfig& a) {
    std::vector<std::string>& files = a.getSourceFiles();
    try {
        files = j.at("source_files").get<std::vector<std::string>>();
    } catch (...) {
        files = {};
    }

    try {
        a.setExecutable(j.at("executable"));
    } catch (...) {
        a.setExecutable("");
    }

    try {
        a.setToolID(j.at("tool_id"));
    } catch (...) {
        a.setToolID("None");
    }

    try {
        a.setVersion(j.at("version"));
    } catch (...) {
        a.setVersion(newVersion);
    }

    std::vector<ToolAction>& actions = a.getActions();
    try {
        actions = j.at("actions").get<std::vector<ToolAction>>();
    } catch (...) {
        actions = {};
    }
}

