#ifndef MACROPHAGE_PROLIFERATION_H_
#define MACROPHAGE_PROLIFERATION_H_

#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"



namespace bdm {
	//check if the cell is inside the wound area
	struct MacrophageProliferation : public Behavior {
		BDM_BEHAVIOR_HEADER(MacrophageProliferation, Behavior, 4);
	
		public:
			MacrophageProliferation(real_t radius = 1) : squared_radius_(radius * radius) {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (MacrophageProliferation* casted = dynamic_cast<MacrophageProliferation*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("MacrophageProliferation::EventConstructor",
						"other was not of type MacrophageProliferation");
				}
			}
  			
  			void Run(Agent* agent) override {
  				if (auto* this_cell = dynamic_cast<macrophage*>(agent)) {
					bool found_platelet = false;
					auto check_for_platelet = L2F([&](Agent* neighbor, real_t squared_distance) {
						if (squared_distance > squared_radius_) {
							return;
						}
      						if (dynamic_cast<platelet*>(neighbor)) {
        						found_platelet = true;	
      						}
					});
					
    					auto* ctxt = Simulation::GetActive()->GetExecutionContext();
   					ctxt->ForEachNeighbor(check_for_platelet, *this_cell, squared_radius_);

 					if (found_platelet && this_cell->GetInsideWoundArea()) {				
      						this_cell -> AddBehavior(new GrowthDivision(5,5));
      						//this_cell -> SetColor(4);
    					}
    					//check for macrophages 
					
					int nbs_macs = 0;
					auto check_for_macs = L2F([&](Agent* neighbor, real_t squared_distance) {
						if (squared_distance > 20) {
							return;
						}
      						if (dynamic_cast<macrophage*>(neighbor)) {
        						++nbs_macs;
        						
      						}
					});
					
   					ctxt->ForEachNeighbor(check_for_macs, *this_cell, 20);

 					if (nbs_macs > 3) {				
      						//this_cell -> RemoveBehavior(this);
      						//this_cell -> SetColor(5);
      						//std::cout<<nbs_macs<<std::endl;
    					}
    				}
  			}
  			
		private: 
			//cell position
			Real3 position_ ;
			
			real_t squared_radius_ = 0;
			
			
	
	};


}//namespace bdm

#endif // PLATELET_SECRETION_H_
