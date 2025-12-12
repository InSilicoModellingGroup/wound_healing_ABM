#ifndef KERATINOCYTE_H_
#define KERATINOCYTE_H_

#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "agents/fibroblast.h"

namespace bdm {

	class keratinocyte : public wd_cells {
		BDM_AGENT_HEADER(keratinocyte, wd_cells, 3);
		
		public:
			keratinocyte() {}
			explicit keratinocyte(const Real3& position) :Base(position) {}
			
			void Initialize(const NewAgentEvent& event) override { 
    				Base::Initialize(event);
    			
    				if (auto* mother = dynamic_cast<keratinocyte*>(event.existing_agent)) { 
    				
    				//daughter's attributes
    					if (event.GetUid() == CellDivisionEvent::kUid) {
						//cell division event actions
						SetDiameter(2.0);
						SetColor(mother->GetColor());
    						//AddBehavior(new Secretion("VGF", 1));
					}
    			
    				}
    			}
    			void Update(const NewAgentEvent& event) override {
    				Base::Update(event);

    				if (event.GetUid() == CellDivisionEvent::kUid) {
      					//auto* daughter_2 = bdm_static_cast<keratinocyte*>(event.new_agents[0]);
      					//mother attributes
      					
    				}
  			}

		
			//getters and setters 
			void SetActivated(bool a) {is_activated_ = a;}
			bool GetActivated() {return is_activated_;}
			
			void SetIntensity(real_t a) {intensity_ = a;}
			real_t GetIntensity() {return intensity_;}
			

			
	  	private:
	  		// activation for proliferation and migration
	  		bool is_activated_ = false;
	  		
	  		//for connection between keratinocytes
	  		real_t intensity_;
	  		

	  	
	};

}  // namespace bdm

#endif  // KERATINOCYTE_H_
