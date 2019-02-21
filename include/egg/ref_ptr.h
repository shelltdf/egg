#ifndef _REF_PTR_H_
#define _REF_PTR_H_

#include <egg/Export.h>

#include <memory>

//#include<eggRuntime/Referenced.h>

/*
* 已知问题：
*     1 ref_ptr只能强制使用valid接口确定指针有效性
*
*/

namespace egg
{
    //
    class EGG_RUNTIME_EXPORT Referenced
    {
        template<class Other> friend class ref_ptr;

    public:

        Referenced() {}
        virtual ~Referenced() {}

        long getCount() { return shared_ptr.use_count() - 1; }

    private:

        //只允许sp调用
        std::shared_ptr<Referenced>& get_shared_ptr() { return shared_ptr; }
        std::shared_ptr<Referenced> shared_ptr;
    };


    template<class T>
    class ref_ptr
    {
        template<class Other> friend class observer_ptr;

    public:
        ref_ptr(){}

        ref_ptr(T* a)
        {
            ref(a);
        }

        ref_ptr(const ref_ptr& rp)
        {
            ref(rp.get());
        }

        virtual ~ref_ptr()
        {
            if (shared_ptr.get())
            {
                T* v = dynamic_cast<T*>(shared_ptr.get());
                if (v)unref(v);
            }
        }

        ref_ptr& operator = ( const ref_ptr& a )
        {
            if (this->get() == a.get()) return *this;
            if (this->get()) unref(this->get());
            if (a.get()) { ref(a.get()); }
            return (*this);
        }

        inline ref_ptr& operator = (T* ptr)
        {
            if (this->get() == ptr) return *this;//skip
            if (this->get()) unref(this->get());//unref old
            if (ptr) { ref(ptr); }//ref new
            return (*this);
        }

        T& operator*() const /*_NOEXCEPT*/
        {
            return (*get());
        }

        T* operator->() const /*_NOEXCEPT*/
        {
            return (get());
        }

        T* get()const
        {
            T* v = dynamic_cast<T*>(shared_ptr.get());
            return v;
        }

        bool valid()const
        {
            return get() != 0;
        }

        bool operator<(const ref_ptr& rp)const
        {
            if (this->get() < rp.get())
            {
                return true;
            }
            return false;
        }

        T* release()
        {
            if (this->get() == nullptr) return 0;

            T* ret = this->get();
            shared_ptr.reset();

            return ret;
        }

    protected:

        void ref(T* a)
        {
            if (a == nullptr) return;

            //增加引用
            if (!a->get_shared_ptr())
            {
                a->get_shared_ptr() = std::shared_ptr<T>(a);
            }
            shared_ptr = (a->get_shared_ptr());
        }

        void unref(T* a)
        {
            if (a == nullptr) return;

            //删除引用
            shared_ptr.reset();

            //删除引用计算对象
            if (a->get_shared_ptr().get()
                && a->get_shared_ptr().use_count() == 1)
            {
                a->get_shared_ptr().reset();
            }
        }

    private:

        std::shared_ptr<Referenced> shared_ptr;
    };



    template<typename T>
    class observer_ptr
    {
    public:

        observer_ptr() {}
        observer_ptr(T* p)
        {
            ref_ptr<T> rp(p);
            weak_ptr = rp.shared_ptr;
            rp.release();
        }
        observer_ptr(const ref_ptr<T>& rp)
        {
            weak_ptr = rp.shared_ptr;
        }

        virtual ~observer_ptr() {}

        bool operator<(const observer_ptr<T>& rp)const
        {
            //if (weak_ptr.lock() < rp.get().shared_ptr)
#if _MSC_VER > 1900 || EMSCRIPTEN // ver > vc14
            if ((&weak_ptr) < (&rp.weak_ptr))
#else
            if (weak_ptr._Get() < rp.weak_ptr._Get())
#endif
            {
                return true;
            }
            return false;
        }

        ref_ptr<T> get()const
        {
            T* v = dynamic_cast<T*>(weak_ptr.lock().get());
            return (ref_ptr<T>(v));
        }

    private:

        std::weak_ptr<Referenced> weak_ptr;
    };



}//namespace egg

#endif //_REF_PTR_H_
