/* { dg-output "imm_deref\n123\n" } */
extern "C" {
    fn printf(s: *const i8, ...);
}

#[lang = "deref"]
pub trait Deref {
    type Target;

    fn deref(&self) -> &Self::Target;
    // { dg-warning "unused name .self." "" { target *-*-* } .-1 }
    // { dg-warning "unused name .Deref::deref." "" { target *-*-* } .-2 }
}

impl<T> Deref for &T {
    type Target = T;

    fn deref(&self) -> &T {
        unsafe {
            let a = "imm_deref\n\0";
            let b = a as *const str;
            let c = b as *const i8;

            printf(c);
        }

        *self
    }
}

impl<T> Deref for &mut T {
    type Target = T;

    fn deref(&self) -> &T {
        // { dg-warning "unused name" "" { target *-*-* } .-1 }
        unsafe {
            let a = "mut_deref\n\0";
            let b = a as *const str;
            let c = b as *const i8;

            printf(c);
        }

        *self
    }
}

fn main() -> i32 {
    let foo: &i32 = &123;
    let res: i32 = *foo;

    unsafe {
        let a = "%i\n\0";
        let b = a as *const str;
        let c = b as *const i8;

        printf(c, res);
    }

    0
}
