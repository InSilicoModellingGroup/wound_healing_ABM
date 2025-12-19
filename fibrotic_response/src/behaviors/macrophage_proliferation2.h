#ifndef MACROPHAGE_PROLIFERATION2_H_
#define MACROPHAGE_PROLIFERATION2_H_

#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"



namespace bdm {

	struct MacrophageProliferation2 : public Behavior {
		BDM_BEHAVIOR_HEADER(MacrophageProliferation2, Behavior, 11);
	
		public:
			MacrophageProliferation2() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (MacrophageProliferation2* casted = dynamic_cast<MacrophageProliferation2*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("MacrophageProliferation::EventConstructor",
						"other was not of type MacrophageProliferation");
				}
			}
  			
  			void Run(Agent* agent) override {
  				if (auto* this_cell = dynamic_cast<macrophage*>(agent)) {
					if (this_cell->GetCellOnBound() == true) {
      						this_cell->RemoveBehavior(this);
      				}
					
					auto* rm = Simulation::GetActive()->GetResourceManager();
      				auto* dgrid = rm->GetDiffusionGrid(0); //platelet growth factor
      				real_t conc_PDGF = dgrid->GetValue(this_cell->GetPosition());
      					
      					
      				auto* rand = Simulation::GetActive()->GetRandom();

      					
      					if (conc_PDGF > GetProliThreshold()) {
      						if (rand->Uniform() > this_cell->GetProbability()) {
      						     	//std::cout<<conc_PDGF<<std::endl;
      							//std::cout<<10+(4*(conc_PDGF-GetProliThreshold())/(GetProliThreshold()))<<std::endl;
      							this_cell->AddBehavior(new GrowthDivision(1.5,GetProliferationRate()));
      							//this_cell->SetColor(7);
      							
      							int nbs_macs = 0;
							auto check_for_macs = L2F([&](Agent* neighbor, real_t squared_distance) {
								if (squared_distance > squared_distance) {
									return;
								}
      								if (dynamic_cast<macrophage*>(neighbor)) {
        								++nbs_macs;
        						
      								}
							});
				 			auto* ctxt = Simulation::GetActive()->GetExecutionContext();
   							ctxt->ForEachNeighbor(check_for_macs, *this_cell, 1);

 							if (nbs_macs > GetLocalConcDensity()) {
 								auto behaviors = this_cell->GetAllBehaviors();
						   
								    for (auto* b : behaviors) {
									if (dynamic_cast<GrowthDivision*>(b)) {
									    this_cell->RemoveBehavior(b);   // removes *all* GD behaviors
									}
								    }
 												
      								this_cell -> RemoveBehavior(this);
      								//std::cout<<"nbs_macs:"<<nbs_macs<<std::endl;
      								
    							}
      						}
      						
      					}
  
    				}
  			}
  			//getter functions
  			real_t GetProliferationProbability() {return proliferation_probability_;}
  			real_t GetProliThreshold() {return proliferation_threshold_;}
  			real_t GetProliferationRate() {return proliferation_rate_;}
  			real_t GetLocalConcDensity() {return local_conc_density_;}
  			real_t GetLocalPopulationDensity() {return local_population_density_;}
  			
		private: 
			//cell position
			Real3 position_ ;
			
			real_t proliferation_probability_ = 0.1;
			
			real_t proliferation_threshold_ = 80;
			
			real_t proliferation_rate_ = 0.1;
			
			real_t local_conc_density_ = 100; //tgf_b signal
			
			real_t local_population_density_ = 4;
	
	};


}//namespace bdm

#endif // PLATELET_SECRETION_H_
