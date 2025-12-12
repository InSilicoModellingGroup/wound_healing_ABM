#ifndef MACROPHAGE_MIGRATION_H_
#define MACROPHAGE_MIGRATION_H_


#include "biodynamo.h"
#include "agents/primitive_agents/wd_cells.h"
#include "behaviors/random_walk_module.h"


namespace bdm {
	//check if the cell is inside the wound area
	struct MacrophageMigration : public Behavior {
		BDM_BEHAVIOR_HEADER(MacrophageMigration, Behavior, 10);
	
		public:
			MacrophageMigration() {
				AlwaysCopyToNew();
				NeverRemoveFromExisting();
			}
			
			void Initialize(const NewAgentEvent& event) override { //initialize daughters
				Base::Initialize(event);
				auto* other = event.existing_behavior;
				if (MacrophageMigration* casted = dynamic_cast<MacrophageMigration*>(other)) {
					position_ = casted->position_;
				} else {
					Log::Fatal("PlateletSecretion::EventConstructor",
						"other was not of type MacrophageMigration");
				}
			}
  			uint64_t step = Simulation::GetActive()->GetScheduler()->GetSimulatedSteps();
  			
  			void Run(Agent* agent) override {
  				if (auto* mac = dynamic_cast<macrophage*>(agent)) {
  					
  				if (mac->GetCellOnBound() == true) {
      					mac->RemoveBehavior(this);
      				}
      				else {
      					mac->AddBehavior(new RandomWalk(0.5));
      				}
      				auto* rand = Simulation::GetActive()->GetRandom();
      				
      				//std::cout<<mac->GetProbability()<<std::endl;
      				
  					if (mac->GetProbability() > rand->Uniform()){
  						mac->AddBehavior(new Chemotaxis("PDGF", GetMigrationRate()));

  					}		
  				}
  			}
  			real_t GetMigrationRate() {return migration_rate_;}
  			real_t GetMigrationProbability() {return migration_propability_;}
		private: 
			//cell position
			Real3 position_ ;
			
			real_t migration_rate_ = 0.005;
			
			real_t migration_propability_ = 0.5;
	
	};


}//namespace bdm

#endif // MACROPHAGE_MIGRATION_H_
