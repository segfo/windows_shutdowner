pub struct Timer{
    timeout:std::time::Duration
}

impl    Timer{
    pub fn new(timeout:std::time::Duration)->Self{
        Timer{timeout:timeout}
    }

    pub fn run(
        &self,
        data:&mut std::any::Any,
        callback:fn(data:&mut std::any::Any)->bool
    )->bool{
        self.run_impl(data,callback)
    }

    pub fn run_async(
        &self,
        data:&mut std::any::Any,
        callback:fn(data:&mut std::any::Any)->bool
    )->bool{
        unimplemented!();
    }

    fn run_impl(
        &self,
        data:&mut std::any::Any,
        callback:fn(data:&mut std::any::Any)->bool
    )->bool{
        std::thread::sleep(self.timeout);
        callback(data)
    }
}