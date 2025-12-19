#ifndef MACROPHAGE_SECRETION_H_
#define MACROPHAGE_SECRETION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "agents/macrophage.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct MacrophageSecretion : public Behavior {
		BDM_BEHAVIOR_HEADER(MacrophageSecretion, Behavior, 3);
	
		public:
			MacrophageSecretion() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			virtual ~MacrophageSecretion() {}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (MacrophageSecretion* casted = dynamic_cast<MacrophageSecretion*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("PlateletSecretion::EventConstructor",
						"other was not of type MacrophageSecretion");
				}
			}
  			real_t k =0;
  			void Run(Agent* agent) override {
  				if (auto* cell = dynamic_cast<macrophage*>(agent)) {
  					if (cell->GetCellOnBound() == true) {
      					cell->RemoveBehavior(this);
      					cell->RemoveFromSimulation();
      				}
  						
  					auto* rm = Simulation::GetActive()->GetResourceManager();
      				auto* dgrid = rm->GetDiffusionGrid(0); //platelet growth factor
      				real_t conc_PDGF = dgrid->GetValue(cell->GetPosition());

      				if (conc_PDGF > GetThreshold()) {
      					cell->SetCanDivide(true);
      							
      				}
  			
  					if (cell->GetInsideWoundArea() && cell->GetCanDivide()) {
  						cell->SetCellAge(++k);
    					cell->AddBehavior(new Secretion("TGF-b", GetSecretionRate()));
    						
    				}
      				auto* dgrid_kKGF = rm->GetDiffusionGrid(2);
      				auto* dgrid_TGF = rm->GetDiffusionGrid(1);
      				auto* dgrid_VEGF = rm->GetDiffusionGrid(3);
      				
      				real_t conc_kKGF = dgrid_kKGF->GetValue(cell->GetPosition());
      				real_t conc_TGF = dgrid_TGF->GetValue(cell->GetPosition());
      				real_t conc_VEGF = dgrid_VEGF->GetValue(cell->GetPosition());
      				
      				uint64_t step = Simulation::GetActive()->GetScheduler()->GetSimulatedSteps();
      				
      				if (conc_VEGF > GetApoptosisValue()) {
      					
	      					dgrid_TGF->ChangeConcentrationBy(cell->GetPosition(), -2*conc_TGF);
	      					//std::cout<<"b_macs"<<std::endl;
	      					cell->RemoveBehavior(this);
	      					cell->RemoveFromSimulation();
      					
				}
      					
      					
  				}
  			}
  			
  			real_t GetSecretionRate() {return secretion_rate_;}
  			real_t GetThreshold() {return secretion_threshhold_;}
  			real_t GetApoptosisValue() {return apoptosis_value_;}
		private: 
			//cell position
			Real3 position_ ;
			
			real_t secretion_threshhold_ = 300;
			real_t secretion_rate_ = 0.75;
			
			real_t apoptosis_value_ = 0.3;
	
	};


}//namespace bdm

#endif // MACROPHAGE_SECRETION_H_
