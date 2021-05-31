# include "closure.h"

Closures::Closures(){
    // this->total = total;
}

// Closures::Closures(){
//     // this->total = total;
// }

Items* Closures::find(Items* target){
    for(auto i:closures){
        if(i->indices == target->indices){
            return i;
        }
    }
    return nullptr;
}

// compute the closure(state) and the go function
void Closures::get_closure_and_go(map<char, vector<Items*>>& items, map<char, Symbol*>& symbols){

    bool isChange = true;
    int closure_num = 0;
    deque<Items*> queue;
    set<Item*> tmp;

    Items* startClos = new Items(closure_num);
    Items* crtClos;
    Item* it;

    // S->.X
    it = items[STARTSYM][0]->items[0];
    startClos->append(it,true);
    // items[STARTSYM][0]->items[0]->isContained = true;
    tmp.insert(it);

    // FIXME, requires speeding up
    // create closure for start item
    // A->a.Bb, append B->.X into this closure
    while(!tmp.empty()){
        auto lt = *tmp.begin();

        // if B is non-terminal
        if(lt->dot < lt->length && !lt->get_dot()->is_terminal){
            // get all the items whose lhs with B
            for(auto rt : items[lt->get_dot()->symbol]){

                // append B->.x
                if(!startClos->append(*rt->items.begin(),true)){
                    // (*rt->items.begin())->isContained = true;
                    tmp.insert(*rt->items.begin());
                    tmp.erase(lt);
                }
                else{
                    tmp.erase(lt);
                    break;
                }
            }
        }
        else{
            tmp.erase(lt);
        }
    }
    it = nullptr;

    closures.push_back(startClos);


    // create closure and go
    queue.push_front(startClos);
    while(!queue.empty()){
        crtClos = queue.back();
        queue.pop_back();

        // crtClos->info();
        // cout<<endl;
        for(auto i : symbols){
            Items * newClos = new Items(closure_num+1);
            for(auto item : crtClos->items){
                // if A->a.Xb and the symbol is X, one item can be contained in multiple closures
                if(item->next && item->grammer->tail[item->dot]->symbol == i.first){
                    it = item->next;
                    newClos->append(it,true);

                    // calculate closure for it
                    tmp.clear();
                    tmp.insert(it);
                    // A->a.Bb, append B->.X into this closure
                    while(!tmp.empty()){
                        auto kt = *tmp.begin();

                        // if B is non-terminal
                        if(kt->next && !kt->grammer->tail[kt->dot]->is_terminal){
                            // get all the items whose lhs with B
                            for(auto rt : items[kt->grammer->tail[kt->dot]->symbol]){
                                // append B->.x
                                if(!newClos->append(*rt->items.begin(), true)){
                                    // (*rt->items.begin())->isContained = true;
                                    tmp.insert(rt->items[0]);
                                    tmp.erase(kt);
                                }
                                else{
                                    tmp.erase(kt);
                                    break;
                                }
                            }
                        }
                        else{
                            tmp.erase(kt);
                        }
                    }

                    startClos = find(newClos);
                    if(!startClos){
                        queue.push_front(newClos);
                        closures.push_back(newClos);
                        crtClos->route[i.first] = newClos;
                        closure_num++;
                    }
                    else{
                        crtClos->route[i.first] = startClos;
                    }

                }
            }
        }
    }

    length = closures.size();
}

void Closures::info(){
    for(auto i : closures){
        cout<<"Closure No."<<i->index<<": ";
        i->info();

        cout<<"Route:";
        for(auto j : i->route){
            cout<<'['<<j.first<<" -> "<<j.second->index<<"], ";
        }
        cout<<endl;
    }
}