#ifndef KERATINOCYTE_SECRETION_H_
#define KERATINOCYTE_SECRETION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct KeratinocyteSecretion : public Behavior {
		BDM_BEHAVIOR_HEADER(KeratinocyteSecretion, Behavior, 7);
	
		public:
			KeratinocyteSecretion() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (KeratinocyteSecretion* casted = dynamic_cast<KeratinocyteSecretion*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("FibroblastSecretion::EventConstructor",
						"other was not of type FibroblastSecretion");
				}
			}
  			void Run(Agent* agent) override {
  				if (auto* cell = dynamic_cast<keratinocyte*>(agent)) {
  					if (cell->GetCellOnBound() == true) {		  					
		  					cell->RemoveFromSimulation();
		  			}
  					auto* rm = Simulation::GetActive()->GetResourceManager();
      					auto* dgrid_KGF = rm->GetDiffusionGrid(2);
      					real_t conc_KGF = dgrid_KGF->GetValue(cell->GetPosition());
      						
      					if (conc_KGF > GetThreshold()) {
      						cell->SetCanDivide(true);
      							
      							
      					}
  						if (cell->GetInsideWoundArea() && cell->GetCanDivide()) {
    						cell->AddBehavior(new Secretion("VGF", secretion_rate_));	
    					}
  				}
 
  			//std::cout<<"deactivated_krt:"<< deactivated_krt << " for:"<< deactivated_krt <<std::endl;
  			}
  			
  			real_t GetSecretionRate() {return secretion_rate_;}
  			real_t GetThreshold() {return secretion_threshhold_;}
  			real_t GetApoptosisValue() {return apoptosis_value_;}
  			
		private: 
			//cell position
			Real3 position_ ;
			
			real_t secretion_threshhold_ = 200;
			
			real_t secretion_rate_ = 0.2;
			
			real_t apoptosis_value_ = 0.2;
	
	};


}//namespace bdm

#endif // KERATINOCYTE_SECRETION_H_
