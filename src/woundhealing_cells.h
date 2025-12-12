// -----------------------------------------------------------------------------

// Copyright (C) 2021 CERN & University of Surrey for the benefit of the
// BioDynaMo collaboration. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------
#ifndef WOUNDHEALING_CELLS_H_
#define WOUNDHEALING_CELLS_H_


#include "biodynamo.h"

#include "agents/primitive_agents/wd_cells.h"
#include "agents/platelet.h"
#include "agents/macrophage.h"
#include "agents/fibroblast.h"
#include "agents/keratinocyte.h"
#include "agents/epithelial_cell.h"

#include "behaviors/inside_wound_area.h"
#include "behaviors/random_walk_module.h"
#include "voronoi_constructor.h"

#include "behaviors/platelet_secretion.h"

#include "behaviors/macrophage_migration.h"
#include "behaviors/macrophage_secretion.h"
#include "behaviors/macrophage_proliferation2.h"

#include "behaviors/fibroblast_proliferation2.h"
#include "behaviors/fibroblast_secretion.h"
#include "behaviors/fibroblast_migration.h"


#include "behaviors/keratinocyte_migration.h"
#include "behaviors/keratinocyte_proliferation.h"
#include "behaviors/keratinocyte_secretion.h"

namespace bdm {

enum CellType { kplatelet, kmacrophage, kfibroblast, kkeratinocyte };
enum Substances { kPDGF, kTGFb, kKGF, kVGF };


inline int Simulate(int argc, const char** argv) {
	auto set_param = [](Param* param) {
    	// Create an artificial bound for the simulation space
    		
    		param->use_progress_bar = true;
			
    		param->bound_space = Param::BoundSpaceMode::kClosed;
    		param->min_bound = -50;
    		param->max_bound = 150;
    		
  	};
  	
  	
	Simulation simulation(argc, argv, set_param);

	
	auto* ts = simulation.GetTimeSeries();
  	auto get_num_macs = [](Simulation* sim) {
    	auto condition = L2F([](Agent* a) {
     	 	if (auto mac = dynamic_cast<macrophage*>(a)) {
        		return true;
     	 	}
      			return false;
    		});
    auto result = static_cast<real_t>(bdm::experimental::Count(sim, condition));
    return result;
  	};
  	
  	auto get_num_fibs = [](Simulation* sim) {
    auto condition = L2F([](Agent* a) {
     	 	if (auto fib = dynamic_cast<fibroblast*>(a)) {
        		return true;
     	 	}
      			return false;
    		});
    auto result_fibs = static_cast<real_t>(bdm::experimental::Count(sim, condition));
    return result_fibs;
  	};
  	
  	auto get_num_krts = [](Simulation* sim) {
    auto condition = L2F([](Agent* a) {
     	 	if (auto krt = dynamic_cast<keratinocyte*>(a)) {
        		return true;
     	 	}
      			return false;
    		});
    auto result_krt = static_cast<real_t>(bdm::experimental::Count(sim, condition));
    return result_krt;
  	};
  	
  	auto get_num_plts = [](Simulation* sim) {
    auto condition = L2F([](Agent* a) {
     	 	if (auto plt = dynamic_cast<platelet*>(a)) {
        		return true;
     	 	}
      			return false;
    		});
    auto result_plt = static_cast<real_t>(bdm::experimental::Count(sim, condition));
    return result_plt;
  	};
 	ts->AddCollector("Num_Macs", get_num_macs);
	ts->AddCollector("Num_Fibs",get_num_fibs);
	ts->AddCollector("Num_Krts",get_num_krts);
	ts->AddCollector("Num_Plts",get_num_plts);
	
	
  	//auto* random = simulation.GetRandom();
  	auto* rm = simulation.GetResourceManager();
	
	
  	
  	ModelInitializer::DefineSubstance(Substances::kPDGF, "PDGF", 0.0, 0.9, 50);
  	ModelInitializer::DefineSubstance(Substances::kTGFb, "TGF-b",0.0, 0.9, 50);
  	ModelInitializer::DefineSubstance(Substances::kKGF, "KGF",0.0, 0.9, 50);
  	ModelInitializer::DefineSubstance(Substances::kVGF, "VGF",0.0, 0.9, 50);
  	


	auto plt_builder = [&](const Real3& position) { //platelet builder
    		auto* cell = new platelet(position);
    		cell->SetDiameter(1);
    		//cell->SetMass(2);
    		cell->AddBehavior(new InsideWoundArea());
    		cell->AddBehavior(new PlateletSecretion());
    		//cell->AddBehavior(new RandomWalk(1));
    		cell->SetCanDivide(false);
    		cell->SetColor({1, 0.75, 0.79}); //red color
    		cell->SetVoronoiCell(false);

    	return cell;
  	};
  	
  	auto mac_builder = [&](const Real3& position) { //macrophage builder
    		auto* cell = new macrophage(position);
    		cell->SetDiameter(1);//Adding characteristics is critical, Diameter->insidewoundarea()->chemotaxis()->divide()->secretion/prolif
    		//cell->SetMass(3);
    		cell->AddBehavior(new InsideWoundArea());
    		
    		cell->AddBehavior(new MacrophageMigration());
    		cell->AddBehavior(new MacrophageSecretion());
    		cell->AddBehavior(new MacrophageProliferation2());
    		
    		cell->SetCanDivide(false);
    		cell->SetColor({0, 0, 1}); //blue color
    	return cell;
  	};
  	
  	auto krt_builder = [&](const Real3& position) { //keratinocyte builder
  		auto* cell = new keratinocyte(position);

    		//cell->SetMass(3);
    		cell->AddBehavior(new InsideWoundArea());
    		
    		cell->AddBehavior(new KeratinocyteMigration());
    		cell->AddBehavior(new KeratinocyteProliferation());
    		cell->AddBehavior(new KeratinocyteSecretion());
    		
    		cell->SetColor({0.957, 0.957, 0.859});
    		cell->SetDiameter(2);
    		cell->SetCanDivide(false);
    		
  	return cell;
  	};
  	
  	auto fbr_builder = [&](const Real3& position) { //fibroblast builder
  		auto* cell = new fibroblast(position);
    		cell->SetDiameter(1);
    		cell->AddBehavior(new InsideWoundArea());
    		
    		cell->AddBehavior(new FibroblastMigration());
    		cell->AddBehavior(new FibroblastSecretion());
    		cell->AddBehavior(new FibroblastProliferation2());
    		
    		cell->SetCanDivide(false);
    		cell->SetColor({0, 1, 0});   		
  	return cell;
  	};
  	

  	
  	auto edge_positions = GetVoronoiEdgePositions<epithelial_cell>(
    	voronoi_seeds,
    	{1,1,1}, {99,99,99},
    	0.8, 0.8, 0.8, 1, 1, 42);
	/*
	real_t grid_dx, real_t plane_eps, real_t gap_epsilon, real_t jitter
    	*/
	// print or use positions
	//std::cout << "Found " << edge_positions.size() << " Voronoi edge points.\n";

	std::vector<Real3> vor_pos;
	std::ofstream file_vor("src/positions/vor_pos.txt");
	for (auto& pos : edge_positions) {
		if (pos[1] > abs(2*pos[0] - 100) + 40 || pos[1] > 70) {
			continue;
		}
		auto* voronoi_cell = new epithelial_cell(pos);
		//voronoi_cell ->SetVoronoiCell(true);
		//voronoi_cell ->SetRestVorCell(false);
  		voronoi_cell->SetDiameter(1);
  		voronoi_cell->SetColor({1, 0, 0});
  		vor_pos.push_back(pos);
		rm->AddAgent(voronoi_cell);
		file_vor << std::fixed << std::setprecision(6);
	   	file_vor << pos[0] << " " << pos[1] << " " << pos[2] << "\n";
	}
	file_vor.close();
	
	/*
	/////////////////////////////////////////////////////////////////////////////////////////// 
	READ POSITIONS OF CELLS
	EACH POSITION IS LOCATED IN src/positons/***_pos.txt
	//////////////////////////////////////////////////////////////////////////////////////////
	*/
	
	std::ifstream file_krt("src/positions/krt_pos.txt");
	std::ifstream file_plt("src/positions/plt_pos.txt");
	std::ifstream file_fbr("src/positions/fbr_pos.txt");
	std::ifstream file_mac("src/positions/mac_pos.txt");

	if (!file_krt.is_open() || !file_fbr.is_open() || !file_mac.is_open() || !file_plt.is_open() ) {
		std::cerr << "Error: could not open file\n";
		return 1;
	}

	std::vector<bdm::Real3> krt_pos;  // store all coordinates here
	std::vector<bdm::Real3> plt_pos;
	std::vector<bdm::Real3> fbr_pos;
	std::vector<bdm::Real3> mac_pos;
	
	double x, y, z;
	int i = 0;
	// Read until the end of file
	while (file_krt >> x >> y >> z) {
		krt_pos.push_back({x, y, z});  // add one position krt
		auto* krt = krt_builder(krt_pos.back());
		if (i == 0) {
		rm->AddAgent(krt);
		i = 1;
		}
		else {
		i = 0;
		}

	}
	while (file_plt >> x >> y >> z) {
		plt_pos.push_back({x, y, z});  // add one position plt
		auto* plt = plt_builder(plt_pos.back());
		rm->AddAgent(plt);
	}
	while (file_fbr >> x >> y >> z) {
		fbr_pos.push_back({x, y, z});  // add one position fbr
		auto* fbr = fbr_builder(fbr_pos.back());
		rm->AddAgent(fbr);
	}
	while (file_mac >> x >> y >> z) {
		mac_pos.push_back({x, y, z});  // add one position mac
		auto* mac = mac_builder(mac_pos.back());
		rm->AddAgent(mac);
	}
	
	
	file_krt.close();
	file_plt.close();
	file_fbr.close();
	file_mac.close();
	/*
	// Just to confirm: print first few positions
	for (size_t i = 0; i < plt_pos.size() && i < 5; i++) {
		std::cout << "Pos " << i << ": "
		          << plt_pos[i][0] << ", "
		          << plt_pos[i][1] << ", "
		          << plt_pos[i][2] << std::endl;
	}

    std::cout << "Total positions read: " << plt_pos.size() << std::endl;
	*/
	std::cout<< simulation.GetActive()->GetScheduler()->GetSimulatedSteps() <<std::endl;

	//Run simulation for one timestep
	simulation.GetScheduler()->Simulate(2000);
	
	experimental::LineGraph macs(ts, "Macrophage Population", "Time", "Number of agents", true, nullptr, 500, 300);

	macs.Add("Num_Macs", "Number of Macrophages", "L", kBlue);
	macs.Draw();
	macs.SetLegendPos(300.0, 300.0, 400.0, 400.0);
	macs.SaveAs(Concat(simulation.GetOutputDir(), "/agent_mac"),
         {".pdf"});
         
	experimental::LineGraph fibs(ts, "Fibroblast Population", "Time", "Number of agents", true, nullptr, 500, 300);
	fibs.Add("Num_Fibs", "Number of Fibroblasts", "L", kRed);
	fibs.SetLegendPos(300.0, 300.0, 400.0, 400.0);
	fibs.Draw();
	
	fibs.SaveAs(Concat(simulation.GetOutputDir(), "/agent_fib"),
         {".pdf"});
         
	experimental::LineGraph plts(ts, "Platelets Population", "Time", "Number of agents", true, nullptr, 500, 300);
	plts.Add("Num_Plts", "Number of Platelets", "L", kBlack);
	plts.Draw();
	plts.SetLegendPos(300.0, 300.0, 400.0, 400.0);
	plts.SaveAs(Concat(simulation.GetOutputDir(), "/agent_plt"),
         {".pdf"});
         
	experimental::LineGraph krts(ts, "Keratinocytes Population", "Time", "Number of agents", true, nullptr, 500, 300);
	krts.Add("Num_Krts", "Number of Keratinocytes", "L", kGreen);
	krts.Draw();
	krts.SetLegendPos(300.0, 300.0, 400.0, 400.0);
	krts.SaveAs(Concat(simulation.GetOutputDir(), "/agent_krt"),
         {".pdf"});
	

	std::cout << "Simulation completed successfully!" << std::endl;
	return 0;
}

}  // namespace bdm

#endif  // WOUNDHEALING_CELLS_H_
