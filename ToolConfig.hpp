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

#ifndef ToolConfig_H
#define ToolConfig_H

#include <vector>
#include <fstream>
#include <iomanip>
#include <string>
#include <cstdlib>

#include "json.hpp"

#define newVersion "1.0"

using json = nlohmann::json;

class ToolAction
{
public:
    ToolAction();

    ToolAction(std::string actionType);

    std::string getActionType();
    void setActionType(std::string type);

    void setName(std::string name);
    std::string getName();

    void setHandle(std::string handle);
    std::string getHandle();

    void setScope(std::string scope);
    std::string getScope();

    void setSourceInfo(std::string source);
    std::string getSourceInfo();

    std::string getFromType();
    void setFromType(std::string type);

    std::string getToType();
    void setToType(std::string type);

    double getError();
    void setError(double error);

    long getAssignments();
    void setAssignments(long asignments);

private:
    std::string action;
    std::string name;
    std::string handle;
    std::string scope;
    std::string sourceInfo;
    std::string fromType;
    std::string toType;
    double error;
    long assignments;
};

void to_json(json &j, const ToolAction &a);
void from_json(const json& j, ToolAction& a);

class ToolConfig
{
public:
    ToolConfig();
    ToolConfig(std::string fileName);

    void addAction(ToolAction action);

    void addReplaceVarType(std::string handle, std::string var_name, std::string scope, std::string source, std::string fromType, std::string toType);
    void addReplaceVarBaseType(std::string handle, std::string var_name, std::string scope, std::string source, std::string fromType, std::string toType);

    void addReplaceVarType(std::string handle, std::string var_name, double error, long assignments);
    void addReplaceVarBaseType(std::string handle, std::string var_name, double error, long assignments);

    std::vector<std::string>& getSourceFiles();

    std::string getExecutable();
    void setExecutable(std::string fileName);

    std::string getToolID();
    void setToolID(std::string toolID);

    std::string getVersion();
    void setVersion(std::string v);

    std::vector<ToolAction>& getActions();

    bool saveConfig(std::string fileName);
private:
    std::vector<std::string> sourceFiles;
    std::string executable;
    std::string toolID;
    std::string version;
    std::vector<ToolAction> actions;
};

void to_json(json &j, const ToolConfig &a);
void from_json(const json& j, ToolConfig& a);

#endif
