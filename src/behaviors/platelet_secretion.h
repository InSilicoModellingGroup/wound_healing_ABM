#ifndef PLATELET_SECRETION_H_
#define PLATELET_SECRETION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct PlateletSecretion : public Behavior {
		BDM_BEHAVIOR_HEADER(PlateletSecretion, Behavior, 2);
	
		public:
			PlateletSecretion() {}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (PlateletSecretion* casted = dynamic_cast<PlateletSecretion*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("PlateletSecretion::EventConstructor",
						"other was not of type PlateletSecretion");
				}
			}
			
  			
      		
  			void Run(Agent* agent) override {
  				if (auto* cell = dynamic_cast<platelet*>(agent)) {
  				  	if (cell->GetCellOnBound() == true) {
      					cell->RemoveBehavior(this);
      				}
      				
  				   	if (cell->GetInsideWoundArea()) {
					    cell->AddBehavior(new Secretion("PDGF", 0.75));
 					}
 					
  					if (cell->GetInitialPositionInWA()) {
    						//cell->SetColor(4);
    						
    				}
    				//concentrations of PDGF and TGF-b
    				auto* rm = Simulation::GetActive()->GetResourceManager();
      				auto* dgrid_TGF = rm->GetDiffusionGrid(1);
      				auto* dgrid_PDGF = rm->GetDiffusionGrid(0);
      				auto* dgrid_KGF = rm->GetDiffusionGrid(2);
      						
      				real_t conc_TGF = dgrid_TGF->GetValue(cell->GetPosition());
  					real_t conc_PDGF = dgrid_PDGF->GetValue(cell->GetPosition());	
					real_t conc_KGF = dgrid_KGF->GetValue(cell->GetPosition());

					
      				if (conc_TGF > GetApoptosisValue() || conc_KGF > GetApoptosisValue()) {	
						dgrid_PDGF->ChangeConcentrationBy(cell->GetPosition(), -conc_PDGF);
						//std::cout<<"removed_subs_plt"<<std::endl;
						cell->RemoveBehavior(this);
						cell->RemoveFromSimulation();
      					}
      				}	
  					

  				
  			}//run function
  			real_t GetApoptosisValue() {return apoptosis_value_;}
		private: 
			//cell position
			Real3 position_ ;
			
			real_t apoptosis_value_ = 0.01;
	
	};


}//namespace bdm

#endif // PLATELET_SECRETION_
