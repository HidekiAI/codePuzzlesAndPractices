/// An operation to perform on two subexpressions.
#[derive(Debug)]
enum Operation {
    Add,
    Sub,
    Mul,
    Div,
}

/// An expression, in tree form.
// DiscriminatedUnion of either Op or Value
#[derive(Debug)]
enum Expression {
    /// An operation on two subexpressions.
    Op {
        op: Operation,
        left: Box<Expression>,
        right: Box<Expression>,
    },

    /// A literal value
    Value(i64),
}

fn eval(expr_du: Expression) -> Result<i64, String> {
    match expr_du {
        Expression::Value(v) => Ok(v),
        Expression::Op { op, left, right } => {
            let l = match eval(*left) {
                Ok(v) => v,
                Err(e) => return Err(e),
            };
            let r = match eval(*right) {
                Ok(v) => v,
                Err(e) => return Err(e),
            };
            match op {
                Operation::Add => {
                    return Ok(l + r);
                }
                Operation::Sub => Ok(l - r),
                Operation::Mul => Ok(l * r),
                Operation::Div => 
                match r {
                    0 => return Err(String::from("division by zero")),
                    _ => return Ok(l / r),
                }
            }
        }
    }
}

#[test]
fn test_value() {
    assert_eq!(eval(Expression::Value(19)), Ok(19));
}

#[test]
fn test_sum() {
    assert_eq!(
        eval(Expression::Op {
            op: Operation::Add,
            left: Box::new(Expression::Value(10)),
            right: Box::new(Expression::Value(20)),
        }),
        Ok(30)
    );
}

#[test]
fn test_recursion() {
    let term1 = Expression::Op {
        op: Operation::Mul,
        left: Box::new(Expression::Value(10)),
        right: Box::new(Expression::Value(9)),
    };
    let term2 = Expression::Op {
        op: Operation::Mul,
        left: Box::new(Expression::Op {
            op: Operation::Sub,
            left: Box::new(Expression::Value(3)),
            right: Box::new(Expression::Value(4)),
        }),
        right: Box::new(Expression::Value(5)),
    };
    assert_eq!(
        eval(Expression::Op {
            op: Operation::Add,
            left: Box::new(term1),
            right: Box::new(term2),
        }),
        Ok(85)
    );
}

#[test]
fn test_error() {
    assert_eq!(
        eval(Expression::Op {
            op: Operation::Div,
            left: Box::new(Expression::Value(99)),
            right: Box::new(Expression::Value(0)),
        }),
        Err(String::from("division by zero"))
    );
}

fn main() {}
