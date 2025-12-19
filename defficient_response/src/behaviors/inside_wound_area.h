#ifndef INSIDE_WOUND_AREA_H_
#define INSIDE_WOUND_AREA_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "agents/macrophage.h"
#include "agents/platelet.h"
#include "agents/keratinocyte.h"
#include "agents/fibroblast.h"
#include "agents/epithelial_cell.h"

#include <mutex>

#include "behaviors/platelet_secretion.h"

#include "behaviors/macrophage_proliferation2.h"
#include "behaviors/macrophage_secretion.h"
#include "behaviors/macrophage_migration.h"


namespace bdm {
	//check if the cell is inside the wound area
	
	static std::vector<Real3> removed_voronoi_positions;
	static std::mutex removed_voronoi_mtx;
	
	struct InsideWoundArea : public Behavior {
		BDM_BEHAVIOR_HEADER(InsideWoundArea, Behavior, 1);
	
		public:
			InsideWoundArea() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
		 
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (InsideWoundArea* casted = dynamic_cast<InsideWoundArea*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("InsideWoundArea::EventConstructor",
						"other was not of type InsideWoundArea");
				}
			}
  			
  			 
  			void Run(Agent* agent) override {
  				if (auto* cell = dynamic_cast<wd_cells*>(agent)) {
  					if (dynamic_cast<epithelial_cell*>(cell)) {
  						cell->UpdatePosition({0,0,0});
  					
  					}
  					//check if is inside wound area
  					//if the coordinates are inside the V-cut shape
  					// position[2] > std::abs(-2 * position[1] + 100) + 20  <--Inside wound 
  					//position[2] < std::abs(-2 * position[1] + 100) + 20  <--outside wound

					auto* param = Simulation::GetActive()->GetParam();
  					
  					
  					Real3 xyz = cell->GetPosition();
          			const real_t min_b = param->min_bound + 50 + cell->GetDiameter();
         			const real_t max_b = param->max_bound - 50 - cell->GetDiameter();
         
  					if (xyz[0] < min_b) {
						xyz[0] = min_b;
						cell->SetCellOnBound(true);
					} else if (xyz[0] > max_b) {
						xyz[0] = max_b;
						cell->SetCellOnBound(true);
					}
					  
					if (xyz[1] < min_b) {
						xyz[1] = min_b;
						cell->SetCellOnBound(true);
					} else if (xyz[1] > max_b) {
						xyz[1] = max_b;
						cell->SetCellOnBound(true);
					}
					
					if (xyz[2] < min_b) {
						xyz[2] = min_b;
						cell->SetCellOnBound(true);
					} else if (xyz[2] > max_b) {
						xyz[2] = max_b;
						cell->SetCellOnBound(true);
					}
					

    					if (cell->GetPosition()[1] > abs(2*cell->GetPosition()[0] - 100) + 40) {  //Constant Behavior during the whole simulation
    						cell->SetInsideWoundArea(true);
    					}
    					else {
    						cell->SetInsideWoundArea(false);
    					}	
    					
    						if (cell->GetPosition()[1] < abs(2*cell->GetPosition()[0] - 100) + 28 || cell->GetPosition()[1] > max_b) {  //Constant Behavior during the whole simulation for macs and fbrs not to escape v-cut
								if (auto* mac = dynamic_cast<macrophage*>(cell)) {
    								mac->SetCellOnBound(true);	
    							}
    							if (auto* fbr = dynamic_cast<fibroblast*>(cell)) {
    								fbr->SetCellOnBound(true);	
    							}	
    						}		
				}
			}
    			   
	
  					
  					
  				
  			
		private: 
			//cell position
			Real3 position_ ;
	
	};


}//namespace bdm

#endif // inside_wound_area.h
