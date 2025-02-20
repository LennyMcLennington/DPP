/************************************************************************************
 *
 * D++, A Lightweight C++ library for Discord
 *
 * Copyright 2021 Craig Edwards and D++ contributors 
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#include <dpp/invite.h>
#include <dpp/discordevents.h>
#include <dpp/nlohmann/json.hpp>

using json = nlohmann::json;

namespace dpp {

invite::invite() : guild_id(0), channel_id(0), inviter_id(0), target_user_id(0), target_user_type(1), approximate_presence_count(0), approximate_member_count(0)
{
}

invite::~invite() = default;


invite& invite::fill_from_json(nlohmann::json* j) {
	code = StringNotNull(j, "code");
	guild_id = (j->find("guild") != j->end()) ? SnowflakeNotNull(&((*j)["guild_id"]), "id") : 0;
	channel_id = (j->find("channel") != j->end()) ? SnowflakeNotNull(&((*j)["channel"]), "id") : 0;
	inviter_id = (j->find("inviter") != j->end()) ? SnowflakeNotNull(&((*j)["inviter"]), "id") : 0;
	target_user_id = (j->find("target_user") != j->end()) ? SnowflakeNotNull(&((*j)["target_user"]), "id") : 0;
	target_user_type = Int8NotNull(j, "target_user_type");
	approximate_presence_count = Int32NotNull(j, "approximate_presence_count");
	approximate_member_count = Int32NotNull(j, "approximate_member_count");
	max_age = Int32NotNull(j, "max_age");
	max_uses = Int32NotNull(j, "max_uses");
	temporary = BoolNotNull(j, "temporary");
	unique = BoolNotNull(j, "unique");
	return *this;
}

std::string invite::build_json() const {
	json j;
	if (max_age > 0)
		j["max_age"] = max_age;
	if (max_uses > 0)
		j["max_uses"] = max_uses;
	if (target_user_id > 0)
		j["target_user"] = target_user_id;
	if (target_user_type > 0)
		j["target_user_type"] = target_user_type;
	if (temporary)
		j["temporary"] = temporary;
	if (unique)
		j["unique"] = unique;
	return j.dump();
}

};
