This is my examination project for the course due 30-06-2026.
This project explores the Levenberg–Marquardt Trust-Region Damped Newton Minimizer introduced in
(http://fedorov.sdf.org/prog/projex/levenberg-trust.htm).
This README briefly summarizes what has been implemented and the main results.

1. ALGORITHMS
The project has 2 primary implemented algorithms, the standard Newton minimzation using back-tracking
line search, and the Levenberg–Marquardt Trust-Region Damped Newton Minimizer (LM). The standard Newton
was enhanced with an option to revert to a simple gradient descent in case of a step failing multiple 
times, this version will be referred to as LS + Fallback.
The LM implementation is also split in two, a simple implementation where λ is always increased/decreased
by 2 when a step is rejected/accepted, and a more sophisticated version depending on the ratio between the 
reduction in the actual objective function and that predicted by the model. More precisely the calculation 
of λ is (λ *= max(1/3, 1 - (2*ratio - 1)^3)); nu = 2; if the step succeeds, and (λ *= nu; nu *=2) if
it fails, inspired by equation 2.21 in 
METHODS FOR NON-LINEAR LEAST SQUARES PROBLEMS 2nd Edition, April 2004 K. Madsen, H.B. Nielsen, O. Tingleff
(https://www2.imm.dtu.dk/pubdb/edoc/imm3215.pdf).


2. RESULTS - This can be skipped if "Out.txt" is read (which includes explicit results).
LM was found to generally be more stable than a simple Newton minimizer, generally using fewer iterations
and function evaluations. This is mainly believed to stem from its built-in reversion to gradient descent 
as λ grows large. This is supported by the fact that the Fallback method generally performed closer to
the LM methods. A notable exception is the Rosenbrock function with a minimization process started at
(-100,100), where both Newton methods failed. This is likely due to the x^4 dependence of the Rosenbrock
function leading to an ill-behaved Hessian matrix causing vast oversteps avoid by the trust region LM methods. 
This hypothesis is supported by the fact that the Fallback method solved the problem on par with LM when 
only the initial y-value was large, i.e. at point (2,100).
Even though Newton generally seemed to use more iterations than the LM models, it should be noted that
it only makes one call to the QR-solver per iteration, while the LM models can make multiple - calling
it every reevaluation of λ. 
This means that while the Newton methods generally make more function calls, the LM methods tend to 
make more QR-solver calls. Since the QR solving process scales as O(N^3) (for square matrices),
the LM methods could quickly become infeasible for large parameter spaces.
On the opposite end, the Newton methods would become slow when dealing with highly complex functions as they
need to evaluate the function every time they reevaluate λ. 
Furthermore we saw that the trust region can help prevent oversteps caused by an ill-conditioned
Hessian where the Newton methods would fail completely. 
In regards to prefered values for the initial damping parameter it was found that for a reasonable start 
guess for the starting position of the minimizer, a λ value not too large nor too small was optimal, 
while for a guess far from the minimum large values was beneficial. The algorithm did however prove 
rather stable to changes in initial damping parameter as it always converged, only the speed was affected.

3. EVALUATION
I believe we were told to self-evaluate the project, hence this short segment. 
The project implements the base algorithm, the optional extension to the algorithm and a slight extension
to the base Newton algorithm. Furthermore it thoroughly compares the different algortihms, exploring when 
they fail and theorising why this is so. As such i believe the project is rated somewhere around ~9-10 / 10.
