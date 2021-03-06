{
    fvScalarMatrix TEqn
    (
        fvm::ddt(rho, T) + fvm::div(rhoPhi, T)
      - fvm::Sp(contErr, T)
      - fvm::laplacian(mixture.alphaEff(turbulence->mut()), T)
      + (
            (
                fvc::div(fvc::absolute(phi, U), p)
              + fvc::ddt(rho, K) + fvc::div(rhoPhi, K)
            )()() - contErr*K
        )
       *(
           alpha1/mixture.thermo1().Cv()
         + alpha2/mixture.thermo2().Cv()
        )()()
     ==
        fvOptions(rho, T)
      + pos(Srho)
       *(
            surfaceFilm.Sh()()/mixture.thermo1().Cp()()
          + surfaceFilm.Tref*Srho
        )
    );

    TEqn.relax();

    fvOptions.constrain(TEqn);

    TEqn.solve();

    fvOptions.correct(T);

    mixture.correctThermo();
    mixture.correct();
}
