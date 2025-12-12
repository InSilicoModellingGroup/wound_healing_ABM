#ifndef MACROPHAGE_H_
#define MACROPHAGE_H_

#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"

namespace bdm {

	class macrophage : public wd_cells {
		BDM_AGENT_HEADER(macrophage, wd_cells, 2);
		
		public:
			macrophage() {}
			explicit macrophage(const Real3& position) :Base(position) {}
			virtual ~macrophage() {}
			
			void Initialize(const NewAgentEvent& event) override { 
    				Base::Initialize(event);
    			

    				
    				if (auto* mother = dynamic_cast<macrophage*>(event.existing_agent)) { 
    					//daughter's attributes
    					
    					if (event.GetUid() == CellDivisionEvent::kUid) {
						SetColor(mother->GetColor());
						//AddBehavior(new MyChemotaxis("PDGF", 0.02));
						SetProbability(mother->GetProbability());
						//this->AddBehavior(new InsideWoundArea());
					}
    			
    				}
    			}

			void SetProlifTimes(int a) { times_proliferated_ = a ;}
			int GetProlifTimes() { return times_proliferated_ ;}
			
	  	private:

	  		real_t times_proliferated_ = 1;	
	  		
	  			
	  	
	  	
	};

}  // namespace bdm

#endif  // MACROPHAGE_H_
