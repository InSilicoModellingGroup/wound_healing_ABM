#ifndef FIBROBLAST_SECRETION_H_
#define FIBROBLAST_SECRETION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "behaviors/fibroblast_migration.h"
#include "behaviors/fibroblast_proliferation2.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct FibroblastSecretion : public Behavior {
		BDM_BEHAVIOR_HEADER(FibroblastSecretion, Behavior, 5);
	
		public:
			FibroblastSecretion() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (FibroblastSecretion* casted = dynamic_cast<FibroblastSecretion*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("FibroblastSecretion::EventConstructor",
						"other was not of type FibroblastSecretion");
				}
			}
  			
  				void Run(Agent* agent) override {
  					if (auto* cell = dynamic_cast<fibroblast*>(agent)) {
  					
	  					if (cell->GetCellOnBound() == true) {
		  					cell->RemoveBehavior(this);
		  					cell->RemoveFromSimulation();
		  				}
		  				
  						auto* rm = Simulation::GetActive()->GetResourceManager();
      					auto* dgrid_TGF = rm->GetDiffusionGrid(1);
      					real_t conc_TGF = dgrid_TGF->GetValue(cell->GetPosition());
      						
      					if (conc_TGF > GetThreshold()) {
      						cell->SetCanDivide(true);
      						dgrid_TGF->ChangeConcentrationBy(cell->GetPosition(), -conc_TGF);
      					}
  						if (cell->GetInsideWoundArea() && cell->GetCanDivide()) {
    						cell->AddBehavior(new Secretion("KGF", GetSecretionRate()));	
    					}
    						
    					auto* dgrid_kKGF = rm->GetDiffusionGrid(2);
      					auto* dgrid_VGF = rm->GetDiffusionGrid(3);
      					real_t conc_kKGF = dgrid_kKGF->GetValue(cell->GetPosition());
      					real_t conc_VGF = dgrid_VGF->GetValue(cell->GetPosition());
      						
      					if (conc_VGF > GetApoptosisValue()) {
      						dgrid_kKGF->ChangeConcentrationBy(cell->GetPosition(), -2*conc_kKGF);
      						
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
			
			real_t apoptosis_value_ = 0.5;
	};


}//namespace bdm

#endif // FIBROBLAST_SECRETION_H_
