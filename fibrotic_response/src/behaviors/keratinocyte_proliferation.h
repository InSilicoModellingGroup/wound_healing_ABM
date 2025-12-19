#ifndef KERATINOCYTE_PROLIFERATION_H_
#define KERATINOCYTE_PROLIFERATION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"


namespace bdm {
	//check if the cell is inside the wound area
	struct KeratinocyteProliferation : public Behavior {
		BDM_BEHAVIOR_HEADER(KeratinocyteProliferation, Behavior, 8);
	
		public:
			KeratinocyteProliferation() {
				AlwaysCopyToNew();

			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (KeratinocyteProliferation* casted = dynamic_cast<KeratinocyteProliferation*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("MacrophageProliferation::EventConstructor",
						"other was not of type MacrophageProliferation");
				}
			}
			
  			
  			void Run(Agent* agent) override {
  				if (auto* this_cell = dynamic_cast<keratinocyte*>(agent)) {
  					uint64_t step = Simulation::GetActive()->GetScheduler()->GetSimulatedSteps();
					auto* rm = Simulation::GetActive()->GetResourceManager();
      					auto* dgrid_KGF = rm->GetDiffusionGrid(2);
      					real_t conc_KGF = dgrid_KGF->GetValue(this_cell->GetPosition());
      					
      					auto* dgrid_vgf = rm->GetDiffusionGrid(3);
      					real_t conc_vgf = dgrid_vgf->GetValue(this_cell->GetPosition());
      					
      					auto* rand = Simulation::GetActive()->GetRandom();
      					      					
      					if (conc_KGF > GetProliThreshold()) {
      						if (rand->Uniform() > GetProliferationProbability()) {
      						
      						this_cell->AddBehavior(new GrowthDivision(2.5,GetProliferationRate()));	  					
    					}
    					int nbs_krts = 0;
						auto check_for_krts = L2F([&](Agent* neighbor, real_t squared_distance) {
							if (squared_distance > 2*2) {
								return;
							}
      							if (dynamic_cast<keratinocyte*>(neighbor)) {
        							++nbs_krts;			
      							}
						});
				 		auto* ctxt = Simulation::GetActive()->GetExecutionContext();
   						ctxt->ForEachNeighbor(check_for_krts, *this_cell, 2*2);
							
				
  						if (nbs_krts > GetLocalPopulationDensity() ) {
						    auto behaviors = this_cell->GetAllBehaviors();
						    //  std::cout << "Num GD behaviors nb_krts= " << this_cell->GetAllBehaviors().size();
						    for (auto* b : behaviors) {
							if (dynamic_cast<GrowthDivision*>(b)) {
							    this_cell->RemoveBehavior(b);   // removes *all* GD behaviors
							}
						    }
							
						    // Now remove KeratinocyteProliferation
						    this_cell->RemoveBehavior(this);
						    //std::cout << " to " << this_cell->GetAllBehaviors().size() << "\n";
						    //this_cell->SetColor({0, 0, 1});
						}		


      							//std::cout<<"nbs_fibs:"<<nbs_krts<<std::endl;
    					}
    					if (conc_vgf > GetLocalConcDensity() || this_cell->GetPosition()[1] < abs(2*this_cell->GetPosition()[0] - 100) + 28)  {//3000=normal, 1000=def, 
						    auto behaviors = this_cell->GetAllBehaviors();
						    // std::cout << "Num GD behaviors step= " << this_cell->GetAllBehaviors().size();
						    for (auto* b : behaviors) {
							if (dynamic_cast<GrowthDivision*>(b)) {
							    this_cell->RemoveBehavior(b);   // remove *all* GD behaviors
							}
						    }
						  
						    
						   // this_cell->SetColor({0, 1, 0});
						   this_cell->SetActivated(true);
						}
					real_t conc_vgf_slope = (7000 - 5*conc_vgf)/2; 
					if (conc_vgf > conc_vgf_slope) {
						auto behaviors = this_cell->GetAllBehaviors();
						   // std::cout << "Num GD behaviors step= " << this_cell->GetAllBehaviors().size();
						    for (auto* b : behaviors) {
							if (dynamic_cast<GrowthDivision*>(b)) {
							    this_cell->RemoveBehavior(b);   // removes *all* GD behaviors
							}
						    }
							//std::cout << " b_growth removed " << "\n";
						    // Now remove KeratinocyteProliferation
						    this_cell->RemoveBehavior(this);
					}		
    				}
  			}//run

  			//Getters
  			real_t GetProliferationProbability() {return proliferation_probability_;}
  			real_t GetProliThreshold() {return proliferation_threshold_;}
  			real_t GetProliferationRate() {return proliferation_rate_;}
  			real_t GetLocalConcDensity() {return local_conc_density_;}
  			real_t GetLocalPopulationDensity() {return local_population_density_;}
  			
		private: 
			Real3 position_ ;
			
			real_t proliferation_probability_ = 0.1;
			
			real_t proliferation_threshold_ = 100;
			
			real_t proliferation_rate_= 1;
			
			real_t local_conc_density_ = 800; //kgf signal
			
			real_t local_population_density_ = 9;
	};


}//namespace bdm

#endif // KERATINOCYTE_PROLIFERATION_H_
