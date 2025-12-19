#ifndef KERATINOCYTE_SECRETION_H_
#define KERATINOCYTE_SECRETION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"

namespace bdm {
	//check if the cell is inside the wound area
	struct KeratinocyteSecretion : public Behavior {
		BDM_BEHAVIOR_HEADER(KeratinocyteSecretion, Behavior, 4);
	
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
						"other was not of type KeratinocyteSecretion");
				}
			}
  			
  			void Run(Agent* agent) override {
  				if (auto* cell = dynamic_cast<keratinocyte*>(agent)) {
  						auto* rm = Simulation::GetActive()->GetResourceManager();
      						auto* dgrid_KGF = rm->GetDiffusionGrid(2);
      						real_t conc_KGF = dgrid_KGF->GetValue(cell->GetPosition());
      						
      						if (conc_KGF > 9000) {
      							cell->SetCanDivide(true);
      						}
  						if (cell->GetInsideWoundArea() && cell->GetCanDivide()) {
    							cell->AddBehavior(new Secretion("VGF", 1));	
    						}
    						
    						auto* dgrid_kKGF = rm->GetDiffusionGrid(2);
      						auto* dgrid_VGF = rm->GetDiffusionGrid(3);
      						real_t conc_kKGF = dgrid_kKGF->GetValue(cell->GetPosition());
      						real_t conc_VGF = dgrid_VGF->GetValue(cell->GetPosition());
      						
      						if (conc_VGF > 15000) {
      							dgrid_kKGF->ChangeConcentrationBy(cell->GetPosition(), -conc_kKGF);
      							cell->RemoveBehavior(this);
      							//cell->RemoveFromSimulation();
      							//cell->SetColor(4);
      						}

      						
	
  				}
  			}
  			
		private: 
			//cell position
			Real3 position_ ;
	
	};


}//namespace bdm

#endif // KERATINOCYTE_SECRETION_H_
