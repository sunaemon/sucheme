#include "environment.h"
#include "gc.h"

struct GCObject;

bool env_map_set(EnvironmentMap *a, int id, GCPtr val)
{
    EnvironmentMap *next = a;
    for(;;) {
        if(id < next->id) {
            if(next->l)
                next=next->l;
            else
                return false;
        } else if(id > next->id) {
            if(next->g)
                next=next->g;
            else
                return false;
        } else if(id == next->id){
            next->val = val;
            return true;
        }
    }
}

void env_map_add(EnvironmentMap *a, int id, GCPtr val)
{
    EnvironmentMap *next = a;
    for(;;) {
        if(id < next->id) {
            if(next->l)
                next=next->l;
            else {
                next->l = alloc_EnvironmentMap(id,val);
                return;
            }
        } else if(id > next->id) {
            if(next->g)
                next=next->g;
            else {
                next->g = alloc_EnvironmentMap(id,val);
                return;
            }
        } else if(id == next->id){
            if(val)
                next->val = val;
            return;
        }
    }
}


void env_define(Environment *e, int id, GCPtr value) {
    if(e->env_map)
        env_map_add(e->env_map, id, value); // copy
    else {
        e->env_map = alloc_EnvironmentMap(id,value);
    }
}

void env_set(Environment *e, int id, GCPtr value) {
    if(e->env_map) {
        if(!env_map_set(e->env_map, id, value)){
            if(e->parent) {
                if(!env_map_set(e->env_map, id, value)) {
                    sprintf(ex_buf, "unbouded_variable:%s", extern_symbol(id));
                    throw_jump();
                }
            } else {
                sprintf(ex_buf, "unbouded_variable:%d", id);
                throw_jump();
            }
                    
        }
    } else {
        e->env_map = alloc_EnvironmentMap(id,value);
    }            
}

void env_map_foreach(void (*c)(int,GCPtr), EnvironmentMap *a) {
    if(a->l)
        env_map_foreach(c,a->l);


    c(a->id,a->val);

    if(a->g)
        env_map_foreach(c,a->g);
}

void env_foreach(void (*c)(int,GCPtr), Environment *e) {
    if(e->env_map)
        env_map_foreach(c,e->env_map);

    if(e->parent)
        env_foreach(c,e);
}

GCPtr env_lookup(const Environment *e, int id)  {
    if(e->env_map) {
        env_map_find_return ret = env_map_find(e->env_map, id);
        if(ret.found)
            return ret.val;
    }
        
    if(e->parent)
        return env_lookup(e->parent, id);
    else {
        sprintf(ex_buf, "unbouded_variable:%s", extern_symbol(id));
        throw_jump();
    }
}

env_map_find_return env_map_find(const EnvironmentMap *a, int id)
{
    const EnvironmentMap *next = a;
    for(;;) {
        if(id < next->id) {
            if(next->l)
                next=(EnvironmentMap*)next->l;
            else {
                env_map_find_return ret = {false, NULL};
                return ret;
            }
        } else if(id > next->id) {
            if(next->g)
                next=(EnvironmentMap*)next->g;
            else {
                env_map_find_return ret = {false, NULL};
                return ret;
            }
        } else if(id == next->id){
            env_map_find_return ret = {true, next->val};
            return ret;
        }
    }
}

bool env_have(const Environment *e, int id)  {
    if(e->env_map)
        return env_map_find(e->env_map, id).found;
    else
        return false;
}

void env_intern_define(Environment *e, const char *name, GCPtr value) {
    env_define(e,intern_symbol(name), value);
}

/*    void env_map_show(EnvironmentMap *a)
      {
      cerr << a->id << " l ";
      if(a->l)
      cerr << a->l->id  << " " << (char*)a->l->val - mem[0];
      cerr << " g ";
      if(a->g)
      cerr << a->g->id << " " << (char*)a->g->val - mem[0];
      cerr << endl;
      if(a->l) {
      env_map_show(a->l);
      }
      if(a->g) {
      env_map_show(a->g);
      }
      }*/
