/*
 * Meds.cpp
 *
 * SCHNAPS
 * Copyright (C) 2009-2011 by Audrey Durand
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

#include "SCHNAPS/SCHNAPS.hpp"
#include "SCHNAPS/Plugins/Meds/Meds.hpp"

SCHNAPS_Plugin_BeginDefinitionM("Meds", "2.0.0");

SCHNAPS_Plugin_AddAllocM("Meds_Event", SCHNAPS::Plugins::Meds::Event::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_PreventionCampaign", SCHNAPS::Plugins::Meds::PreventionCampaign::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_SetBaseQaly", SCHNAPS::Plugins::Meds::SetBaseQaly::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_SetQaly", SCHNAPS::Plugins::Meds::SetQaly::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Test", SCHNAPS::Plugins::Meds::Test::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_Treatment", SCHNAPS::Plugins::Meds::Treatment::Alloc);
SCHNAPS_Plugin_AddAllocM("Meds_UpdateQaly", SCHNAPS::Plugins::Meds::UpdateQaly::Alloc);


SCHNAPS_Plugin_EndDefinitionM();
