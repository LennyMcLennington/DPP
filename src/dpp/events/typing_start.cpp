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
#include <dpp/discord.h>
#include <dpp/event.h>
#include <string>
#include <iostream>
#include <fstream>
#include <dpp/discordclient.h>
#include <dpp/discord.h>
#include <dpp/cache.h>
#include <dpp/stringops.h>
#include <dpp/nlohmann/json.hpp>
#include <dpp/discordevents.h>

using json = nlohmann::json;

namespace dpp { namespace events {

using namespace dpp;

/**
 * @brief Handle event
 * 
 * @param client Websocket client (current shard)
 * @param j JSON data for the event
 * @param raw Raw JSON string
 */
void typing_start::handle(discord_client* client, json &j, const std::string &raw) {
	if (!client->creator->dispatch.typing_start.empty()) {
		json& d = j["d"];
		dpp::typing_start_t ts(client, raw);
		ts.typing_guild = dpp::find_guild(SnowflakeNotNull(&d, "guild_id"));
		ts.typing_channel = dpp::find_channel(SnowflakeNotNull(&d, "channel_id"));
		ts.user_id = SnowflakeNotNull(&d, "user_id");
		ts.typing_user = dpp::find_user(ts.user_id);
		ts.timestamp = TimestampNotNull(&d, "timestamp");
		call_event(client->creator->dispatch.typing_start, ts);
	}
}

}};