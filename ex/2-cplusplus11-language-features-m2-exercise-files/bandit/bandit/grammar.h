#ifndef BANDIT_GRAMMAR_H
#define BANDIT_GRAMMAR_H

namespace bandit {

  using namespace std;
  using namespace detail;

  inline void describe(const char* desc, voidfunc_t func, listener& listener, contextstack_t& context_stack)
  {
    listener.context_starting(desc);

    context_stack.back()->execution_is_starting();

    bandit_context ctxt;
    context_stack.push_back(&ctxt);
    try
    {
      func();
    }
    catch(const test_run_error& error)
    {
      listener.test_run_error(desc, error);
    }

    context_stack.pop_back();

    listener.context_ended(desc);
  }

  inline void describe(const char* desc, voidfunc_t func)
  {
    describe(desc, func, registered_listener(), context_stack());
  }

  inline void before_each(voidfunc_t func, contextstack_t& context_stack)
  {
    context_stack.back()->register_before_each(func);
  }

  inline void before_each(voidfunc_t func)
  {
    before_each(func, context_stack());
  }

  inline void after_each(voidfunc_t func, contextstack_t& context_stack)
  {
    context_stack.back()->register_after_each(func);
  }

  inline void after_each(voidfunc_t func)
  {
    after_each(func, context_stack());
  }

  inline void it(const char* desc, voidfunc_t func, listener& listener,
      contextstack_t& context_stack, bandit::adapters::assertion_adapter& assertion_adapter)
  {
    listener.it_starting(desc);

    context_stack.back()->execution_is_starting();

    auto run_before_eaches = [&](){
      for_each(context_stack.begin(), context_stack.end(), [](context* ctxt){
          ctxt->run_before_eaches();
      });
    };

    auto run_after_eaches = [&](){
      for_each(context_stack.begin(), context_stack.end(), [](context* ctxt){
          ctxt->run_after_eaches();
      });
    };

    try
    {
      assertion_adapter.adapt_exceptions([&](){
          run_before_eaches();

          func();
          listener.it_succeeded(desc);
      });
    }
    catch(const bandit::assertion_exception& ex)
    {
      listener.it_failed(desc, ex);
    }
    catch(...)
    {
      listener.it_unknown_error(desc);
    }

    try
    {
      run_after_eaches();
    }
    catch(...)
    {
      listener.it_unknown_error(desc);
    }
  }

  inline void it(const char* desc, voidfunc_t func)
  {
    it(desc, func, registered_listener(), context_stack(), registered_adapter());
  }

}

#endif
