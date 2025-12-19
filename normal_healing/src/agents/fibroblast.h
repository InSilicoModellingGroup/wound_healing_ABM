#ifndef FIBROBLAST_H_
#define FIBROBLAST_H_

#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"

namespace bdm {

	class fibroblast : public wd_cells {
		BDM_AGENT_HEADER(fibroblast, wd_cells, 4);
		
		public:
			fibroblast() {}
			explicit fibroblast(const Real3& position) :Base(position) {}
			
			void Initialize(const NewAgentEvent& event) override { 
    				Base::Initialize(event);
    				
    				
    				if (auto* mother = dynamic_cast<fibroblast*>(event.existing_agent)) { 
    				//daughter's attributes

    					if (event.GetUid() == CellDivisionEvent::kUid) {
						SetColor(mother->GetColor());
						AddBehavior(new Chemotaxis("TGF-b", 0.002));
						AddBehavior(new Secretion("KGF", 1));

					}
    			
    				}
    			}
    			void Update(const NewAgentEvent& event) override {
    				Base::Update(event);

    				if (event.GetUid() == CellDivisionEvent::kUid) {

      					
      					
    				}
  			}
    			
		
			
	  	private:
	  		// The color that will be used for visualization purposes
	  		
	  	
	  	
	};

}  // namespace bdm

#endif  // fibroblast_H_
