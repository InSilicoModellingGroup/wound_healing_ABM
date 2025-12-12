#ifndef FIBROBLAST_PROLIFERATION_H_
#define FIBROBLAST_PROLIFERATION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "agents/fibroblast.h"
namespace bdm {
	//check if the cell is inside the wound area
	struct FibroblastProliferation : public Behavior {
		BDM_BEHAVIOR_HEADER(FibroblastProliferation, Behavior, 6);
	
		public:
			FibroblastProliferation(real_t radius = 1) : squared_radius_(radius * radius) {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (FibroblastProliferation* casted = dynamic_cast<FibroblastProliferation*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("FibroblastProliferation::EventConstructor",
						"other was not of type fibroblastProliferation");
				}
			}
  			
  			void Run(Agent* agent) override {
  				if (auto* this_cell = (dynamic_cast<fibroblast*>(agent))) {


					
					//proliferation module
					bool found_macrophage = false;
					
					//std::cout <<  this_cell->GetCellType() << std::end;
					auto check_for_macrophage = L2F([&](Agent* neighbor, real_t squared_distance) {
						if (squared_distance > squared_radius_) {
							return;
						}
      						if (dynamic_cast<platelet*>(neighbor)) {
        						found_macrophage = true;
      						}
					});

    					auto* ctxt = Simulation::GetActive()->GetExecutionContext();
   					ctxt->ForEachNeighbor(check_for_macrophage, *this_cell, squared_radius_);


 					if (found_macrophage && this_cell->GetInsideWoundArea()) {
      						//this_cell ->SetColor(6);
      						this_cell->AddBehavior(new GrowthDivision(6,3));
      						
      							
    					}
    				}
  			}
  			
		private: 
			//cell position
			Real3 position_ ;
			
			real_t squared_radius_ = 0;
	
	};


}//namespace bdm

#endif // FIBROBLAST_SECRETION_H_
