       #include "yunus_emre_ertunc.h"
    	#include <stdlib.h>	
    	#include <stdio.h>
	   #include "math.h"
		
		/*
		If you want to see all priority calculations,
		 you just need to delete comment signs at lines 290,295 , 537,543,560
		
			// Yunus Emre Ertunc
			// 150117064
		
		*/

		struct Inputs{
			int id;
			int eValue;
			int e2;
			int time;
			int flag;
			int firstTime;
			int finish;
			int used;
			int waitingTime;
			int totalOperationTime;
			int finishTime;
			double priority;
			double prio2;
			struct Inputs *next;
		};
		typedef struct Inputs inp;
		typedef inp *inpPtr;
		
		struct List{
			inpPtr inp;
			struct List *next;
		};
		typedef struct List list;
		typedef list *listPtr;
		
		struct miniList{
			int id;
			int time;
			struct miniList *next;
		};
		typedef struct miniList miniList;
		typedef miniList *mnPtr;
		
		struct qList{
			int q;
			double awt;
			struct qList *next;
		};
		typedef struct qList qList;
		typedef qList *qPtr;
		
        typedef struct BinNode *Position;

        struct BinNode
        {
		    inpPtr Item;
            Position    LeftChild;
            Position    NextSibling;
        };

        struct Collection
        {
            int CurrentSize;
            BinTree TheTrees[ MaxTrees ];
        };
        
        
        BinQueue Insert2(inpPtr Item, BinQueue H );
        int DeleteMin2( BinQueue H );
		int FindMin2(BinQueue H);
		
        BinQueue
        Initialize( void )
        {
            BinQueue H;
            int i;

            H = malloc( sizeof( struct Collection ) );
            if( H == NULL )
                printf( "Out of space!!!" );
            H->CurrentSize = 0;
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            return H;
        }

        static void
        DestroyTree( BinTree T )
        {
            if( T != NULL )
            {
                DestroyTree( T->LeftChild );
                DestroyTree( T->NextSibling );
                free( T );
            }
        }

        void
        Destroy( BinQueue H )
        {
            int i;

            for( i = 0; i < MaxTrees; i++ )
                DestroyTree( H->TheTrees[ i ] );
        }

        BinQueue
        MakeEmpty( BinQueue H )
        {
            int i;

            Destroy( H );
            for( i = 0; i < MaxTrees; i++ )
                H->TheTrees[ i ] = NULL;
            H->CurrentSize = 0;

            return H;
        }

        BinQueue
        Insert2( inpPtr Item, BinQueue H )  // just parameter is updated
        {
            BinTree NewNode;
            BinQueue OneItem;
			int i;

            NewNode = malloc( sizeof( struct BinNode ) );
            if( NewNode == NULL )
                printf( "Out of space!!!" );
            NewNode->LeftChild = NewNode->NextSibling = NULL;
			NewNode->Item = Item;
			 

            OneItem = Initialize( );
            OneItem->CurrentSize = 1;
            OneItem->TheTrees[ 0 ] = NewNode;

            return Merge( H, OneItem );
        }


        int DeleteMin2( BinQueue H )
        {
            int i, j;
            int MinTree;   // The tree with the minimum item 
            BinQueue DeletedQueue;
            Position DeletedTree, OldRoot;
            double MinItem;

            if( IsEmpty( H ) )
            {
                printf( "Empty binomial queue" );
			
             return 0; 
            }

            MinItem = 500;
            for( i = 0; i < 12; i++ )
            {
                if( H->TheTrees[ i ] && H->TheTrees[ i ]->Item->priority < MinItem )  // a little bit updated
                {
                    // Update minimum 
                    MinItem = H->TheTrees[ i ]->Item->priority;        
					
                    MinTree = i;
                }
            }

            DeletedTree = H->TheTrees[ MinTree ];
            OldRoot = DeletedTree;
            DeletedTree = DeletedTree->LeftChild;
            free( OldRoot );

            DeletedQueue = Initialize( );
            DeletedQueue->CurrentSize = ( 1 << MinTree ) - 1;
            for( j = MinTree - 1; j >= 0; j-- )
            {
                DeletedQueue->TheTrees[ j ] = DeletedTree;
                DeletedTree = DeletedTree->NextSibling;
                DeletedQueue->TheTrees[ j ]->NextSibling = NULL;
            }

            H->TheTrees[ MinTree ] = NULL;
            H->CurrentSize -= DeletedQueue->CurrentSize + 1;

            Merge( H, DeletedQueue );
            return MinItem;
        }


        int
        FindMin2( BinQueue H )   //the smallest item's id is returned
        {
            int i;
            

            if( IsEmpty( H ) )
            {
                printf( "Empty binomial queue" );
				
                return 0;
            }
			int id;
            double MinItem = 500.0;
			
            for( i = 0; i < MaxTrees; i++ )
            {
                if( H->TheTrees[ i ] && H->TheTrees[ i ]->Item->priority < MinItem ) {
                    id = H->TheTrees[ i ]->Item->id;
                    MinItem = H->TheTrees[i]->Item->priority;
				}	
            }
            return id;
        }

        int
        IsEmpty( BinQueue H )
        {
            return H->CurrentSize == 0;
        }

        int IsFull( BinQueue H )
        {
            return H->CurrentSize == Capacity;
        }

        BinTree
        CombineTrees( BinTree T1, BinTree T2 )
        {
            if( T1->Item->priority > T2->Item->priority )                 
                return CombineTrees( T2, T1 );
            if(T1->Item->priority == T2->Item->priority && T1->Item->id > T2->Item->id){ //if priority values equal, smaller one as a time added on the top of the heap
            	return CombineTrees( T2, T1 );
			}
            T2->NextSibling = T1->LeftChild;
            T1->LeftChild = T2;
            return T1;
        }

        BinQueue
        Merge( BinQueue H1, BinQueue H2 )
        {
            BinTree T1, T2, Carry = NULL;
            int i, j;

            if( H1->CurrentSize + H2->CurrentSize > Capacity )
                printf( "Merge would exceed capacity" );

            H1->CurrentSize += H2->CurrentSize;
            for( i = 0, j = 1; j <= H1->CurrentSize; i++, j *= 2 )
            {
                T1 = H1->TheTrees[ i ]; T2 = H2->TheTrees[ i ];

                switch( !!T1 + 2 * !!T2 + 4 * !!Carry )
                {
                    case 0: /* No trees */
                    case 1: /* Only H1 */
                        break;
                    case 2: /* Only H2 */
                        H1->TheTrees[ i ] = T2;
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 4: /* Only Carry */
                        H1->TheTrees[ i ] = Carry;
                        Carry = NULL;
                        break;
                    case 3: /* H1 and H2 */
                        Carry = CombineTrees( T1, T2 );
                        H1->TheTrees[ i ] = H2->TheTrees[ i ] = NULL;
                        break;
                    case 5: /* H1 and Carry */
                        Carry = CombineTrees( T1, Carry );
                        H1->TheTrees[ i ] = NULL;
                        break;
                    case 6: /* H2 and Carry */
                        Carry = CombineTrees( T2, Carry );
                        H2->TheTrees[ i ] = NULL;
                        break;
                    case 7: /* All three */
                        H1->TheTrees[ i ] = Carry;
                        Carry = CombineTrees( T1, T2 );
                        H2->TheTrees[ i ] = NULL;
                        break;
                }
            }
            return H1;
        }
		
		BinTree printTree(BinTree p, BinTree *r, int i)  // just printf methods are updated
		{
		  BinTree t[20]={NULL}, q; int j;
		  for ( j=0; j<i; j++ ) t[j]= r[j];
		  i=0;
		  if (p!=NULL) { 
		//    printf("\t& P%d\t\t\t P%d 's priority :%lf \n",p->Item->id,p->Item->id,p->Item->priority);
			q=p->NextSibling;
			j=0;
			do {
			  while (q!=NULL) { 
		//	    printf("\t& P%d\t\t\t P%d 's priority :%lf\n ",q->Item->id,q->Item->id,q->Item->priority); 
			    if (q->LeftChild != NULL) { r[i]=q->LeftChild; i++; }
			    q=q->NextSibling;
			  }
			  q=t[j++];
			} while (q!=NULL);
		  }
		  else return NULL;
		  printTree(p->LeftChild, r, i);
		}
		
		void insertLL(inpPtr *sPtr,int id,int eValue,int time){
			inpPtr newPtr = malloc(sizeof(inp));
			
			if(newPtr != NULL){
				newPtr->id = id;
				newPtr->eValue = eValue;
				newPtr->e2 = eValue;
				newPtr->time = time;
				newPtr->flag = 0;
				newPtr->firstTime = 0;
				newPtr->finish = 0;
				newPtr->used = 0;
				newPtr->waitingTime = 0;
				newPtr->totalOperationTime = 0;
				newPtr->finishTime = 0;
				newPtr->priority = 0;
				newPtr->prio2 = 0;
				newPtr->next = NULL;
				
				inpPtr previousPtr = NULL;
				inpPtr currentPtr = *sPtr;
				
				while(currentPtr != NULL){
					previousPtr = currentPtr;
					currentPtr = currentPtr->next;
				}
				
				if(previousPtr == NULL){
					newPtr->next = *sPtr;
					*sPtr = newPtr;
				}
				else{
					previousPtr->next = newPtr;
					newPtr->next = currentPtr;
				}
			}
		}
		
		void insertOtherList(listPtr *lPtr,inpPtr inp){
			listPtr newPtr = malloc(sizeof(list));
			
			if(newPtr != NULL){
				newPtr->inp = inp;
			
				listPtr previous = NULL;
				listPtr current = *lPtr;
				
				while(current != NULL){
					previous = current;
					current = current->next;
				}
				
				if(previous == NULL){
					newPtr->next = *lPtr;
					*lPtr = newPtr;
				}
				else{
					previous->next = newPtr;
					newPtr->next = current;
				}
			}
			
		}
		
		void insertqList(qPtr *qq,int q,double awt){
			qPtr newPtr = malloc(sizeof(qList));
			
			if(newPtr != NULL){
				newPtr->q = q;
				newPtr->awt = awt;
				
				qPtr previous = NULL;
				qPtr current = *qq;
				
				while(current != NULL){
					previous = current;
					current = current->next;
				}
				if(previous == NULL){
					newPtr->next = *qq;
					*qq = newPtr;
				}
				else{
					previous->next = newPtr;
					newPtr->next = current;
				}
			}
		}
		
		
		void printLL(inpPtr currentPtr){
			if(currentPtr == NULL){
				puts("List is empty.\n");
			}
			else{
				puts("The list is : ");
				
				while(currentPtr != NULL){
					printf("%d --> ",currentPtr->eValue);
					currentPtr = currentPtr->next;
				}
			}
		}
		
		FILE *inpf;
		
		void readInputFile(inpPtr *sPtr){
			char fline[100];
			
			if((inpf = fopen("input.txt","r")) == NULL){
				printf("Input File Could Not Be Opened!\n");
				return;
			}
			
				int i1,i2,i3;
				while(fgets(fline,sizeof(fline),inpf)){ // line by line okuyor				
					if(3 == sscanf(fline,"%*[^0123456789]%d%*[^0123456789]%d%*[^0123456789]%d",&i1,&i2,&i3)){ //gets number from string	
						insertLL(&(*sPtr),i1,i2,i3);													
					}
				}
		}
		
		void InitiliazeValues(inpPtr sPtr, int *e, int *q){ // determines Emax and q values
			int maxValue = 0;
			
			while(sPtr != NULL){
				if(sPtr->eValue > maxValue){
					maxValue = sPtr->eValue;
				}
				sPtr = sPtr->next;				
			}
			*e = maxValue;
			*q = 10;
		}
		
		listPtr checkLL(inpPtr sPtr,int time){
			listPtr list = NULL;
			while(sPtr != NULL){
				if(sPtr->time <= time && sPtr->used != 1){
					sPtr->flag = 1;
					sPtr->used = 1;
					
					insertOtherList(&list,sPtr);
				}
				sPtr = sPtr->next;
			}
			return list;
		}	
		
		void priority(inpPtr sPtr, BinQueue H1,int q,int Emax, int *time){  // updates the priority of min item and updates the time
			int id;
			if(!IsEmpty(H1)){
				id = FindMin2(H1);
			}
					
			inpPtr temp = sPtr;
				
					
			inpPtr iter = sPtr;
			
			while(iter != NULL){
				if(iter->id == id) break; //item that will be operated is determined
				iter = iter->next;
			}
			
			while(temp != NULL){
				if(temp->flag == 1 && temp->priority == iter->priority && temp->id != iter->id){
					if(temp->time < iter->time){
						id = temp->id;	
						break;					
					}
					else{
						id = iter->id;

					}
				}
				
				temp = temp->next;
			}
			
			inpPtr tt = sPtr;
			while(tt != NULL){
				if(tt->id == id){
					iter = tt;
					break;
				}
				tt = tt->next;
			}
					
			temp = sPtr;
			while(temp != NULL){
				
				if(temp->flag == 1 && temp->priority < iter->priority){
					iter = temp;
				}
			
				temp = temp->next;
			}
			
			if(iter != NULL){ //time is determined
				if(iter->eValue +q >= q){
					*time = *time + q;
					iter->totalOperationTime += q;
				}
				else{
					*time += iter->eValue +q;
					iter->totalOperationTime += iter->eValue+q;
				}
			}
			
			if(iter != NULL){
				iter->priority = iter->eValue * (1.0/(exp((-1)*pow(((2.0*iter->eValue)/(3*Emax)),3))));
				iter->eValue -= q;
				
				
			}
							
		}
		
		initiliazePriority(inpPtr *sPtr,int q){ //This method is to make priority calculation algorithm easier by determining first priority values 
			inpPtr iter = *sPtr;
			
			while(iter != NULL){
				iter->priority = (double)iter->eValue;
				iter->prio2 = (double)iter->eValue;
				iter->eValue -= q;
				iter = iter->next;
			}
			
		}
		
		int finishCondition(inpPtr sPtr){
			while(sPtr != NULL){
				if(sPtr->finish != 1) return 0;
				sPtr = sPtr->next;
			}
			return 1;
			
		}
		
		void startProgram(inpPtr *sPtr,int Emax, int q,BinQueue H1,qPtr *startq){ 
			int i = 0;
			int a = 0;
			int time = 0;
			BinTree p, r[20]={NULL};
			listPtr temp;
			inpPtr temp3 = *sPtr;
			inpPtr temp2 = *sPtr;
			inpPtr iter = *sPtr;
			
			while(temp2->next != NULL){
				temp2 = temp2->next;
			}
			
			for(i = 0;i < q; i++){ // all operations will be repeated as q times
				printf("q = %d\n\n",i+1);
			//	printf("Time\tProcesses in BH\t\tPriority Value of Processes in BH\n------------------------------------------------------------------------\n");
				
				initiliazePriority(&(*sPtr),i+1);
				
				for(;;){  
					if(finishCondition(temp3) == 1) break;  //if all items in linked list are used , then just finish the program
			//		printf("%d",time);
															
					temp = checkLL(*sPtr,time);      //All items at that time interval are hold and then added into the heap
					
					while(temp != NULL){
						Insert2(temp->inp,H1);
						temp = temp->next;
					}
				
	
					for (a=0; a<MaxTrees; a++) {  // print part
	          				p=H1->TheTrees[a];
			  				printTree(p, r, 0); 
						}
						
					priority(*sPtr,H1,i+1,Emax,&time);	// priority calculation
										
				//	printf("\n");
					iter = *sPtr;
					while(iter != NULL){		// delete operation 
						if(iter->priority <= 0.0 && iter->flag == 1){
							DeleteMin2(H1);
							iter->flag = 0;
							iter->finish = 1;
							iter->finishTime = time;
							iter->waitingTime = iter->finishTime - iter->totalOperationTime - iter->time;
						}
						iter = iter->next;
					}
					iter = *sPtr;									
										
				}

				time = 0;
				int numberOfElement	= 0;
				double totalWt = 0.0;	
						
				printf("\nPID\tWaiting time\n-----------------------\n");
				while(iter != NULL){
					printf("P%d\t    %d\n",iter->id,iter->waitingTime);
					numberOfElement++;
					totalWt += iter->waitingTime;
					iter = iter->next;
				}
				
				printf("\nAWT =%.3lf / %d = %.3lf",totalWt,numberOfElement,totalWt/numberOfElement);
				qPtr var = *startq;
				
				insertqList(&(*startq),i+1,totalWt/numberOfElement);
							
				iter = *sPtr;
				while(iter != NULL){  // Linked list turns into its original version 
					iter->eValue = iter->e2;
					iter->priority = iter->prio2;
					iter->used = 0;
					iter->finish = 0;
					iter->waitingTime = 0;
					iter->waitingTime = 0;
					iter->finishTime = 0;
					iter->totalOperationTime = 0;
					iter = iter->next;
				}
			
				printf("\n\n");		
			}
			qPtr var = *startq;
			
			printf("\tq\t\tAWT\n");
			while(var != NULL){
				printf("\t%d\t\t%.3lf\n",var->q,var->awt);
				var = var->next;
			}
		}
						
		main(){
			
		  BinQueue H1;
		  H1 = Initialize( );
          
          qPtr startq = NULL;
		  inpPtr startPtr = NULL;
		  readInputFile(&startPtr);	  
		  
		  int Emax,q;
		  
		  InitiliazeValues(startPtr,&Emax,&q);
		  
		  startProgram(&startPtr,Emax,q,H1,&startq);		  
		    
		  
		}                 

